GfnClient::GfnClient()
    : device_id_(NormalizeCloudDeviceId(GenerateDeviceId()))
{
    if (device_id_.empty())
        throw std::runtime_error("Unable to create a stable GeForce NOW device identity");
}

std::vector<StreamRegion> GfnClient::FetchStreamRegions(AuthSession& session) const
{
    session = RecoverSavedSession(session);
    const std::string base_url = server_location::ResolveStreamingBaseUrl(
        "Auto",
        session.provider.streaming_service_url);
    if (base_url.empty())
        throw std::runtime_error("The GeForce NOW provider has no valid streaming endpoint.");

    const HttpResponse response = http_client_.Get(
        base_url + "v2/serverInfo",
        kUserAgent,
        BuildGfnLcarsHeaders(
            ResolveSessionJwt(session),
            "BROWSER",
            "WEBRTC",
            true));
    if (response.status_code != 200)
    {
        throw std::runtime_error(
            "Could not load GeForce NOW server locations (HTTP " +
            std::to_string(response.status_code) + ").");
    }

    JsonPtr root = LoadJson(response.body);
    json_t* metadata = json_object_get(root.get(), "metaData");
    if (!json_is_array(metadata))
        return {};

    std::vector<StreamRegion> regions;
    std::unordered_set<std::string> seen_urls;
    size_t index = 0;
    json_t* entry = nullptr;
    json_array_foreach(metadata, index, entry)
    {
        const std::string name = Trim(GetString(entry, "key"));
        const std::string raw_url = Trim(GetString(entry, "value"));
        if (name.empty() || name == "gfn-regions" || name.rfind("gfn-", 0) == 0)
            continue;

        const std::string url =
            server_location::NormalizeStreamingBaseUrl(raw_url);
        if (url.empty() || !seen_urls.insert(url).second)
            continue;

        regions.push_back({name, url, -1});
    }

    std::sort(
        regions.begin(),
        regions.end(),
        [](const StreamRegion& left, const StreamRegion& right) {
            return left.name < right.name;
        });
    return regions;
}

std::vector<StreamRegion> GfnClient::MeasureStreamRegionLatencies(
    std::vector<StreamRegion> regions) const
{
    if (regions.empty())
        return regions;

    constexpr long kCoarseTimeoutMs = 700;
    constexpr long kRetryTimeoutMs = 1200;
    constexpr long kRefineTimeoutMs = 900;
    constexpr int kRefineProbeCount = 2;
    constexpr size_t kMaximumFinalists = 5;
    constexpr int kFinalistMarginMs = 50;
    constexpr int kNonFinalistPenaltyMs = 250;

    std::vector<std::vector<int>> samples(regions.size());
    auto probe_indices = [this, &regions, &samples](
                             const std::vector<size_t>& indices,
                             int probe_count,
                             long timeout_ms) {
        if (indices.empty())
            return;

        std::atomic<size_t> next_index {0};
        const size_t worker_count = std::min<size_t>(8, indices.size());
        std::vector<std::thread> workers;
        workers.reserve(worker_count);
        for (size_t worker = 0; worker < worker_count; ++worker)
        {
            workers.emplace_back([&, probe_count, timeout_ms] {
                for (;;)
                {
                    const size_t work =
                        next_index.fetch_add(1, std::memory_order_relaxed);
                    if (work >= indices.size())
                        return;

                    const size_t region_index = indices[work];
                    std::vector<int> measured = MeasureRegionLatencySamples(
                        http_client_, regions[region_index].url,
                        probe_count, timeout_ms);
                    samples[region_index].insert(
                        samples[region_index].end(),
                        measured.begin(), measured.end());
                }
            });
        }
        for (std::thread& worker : workers)
            worker.join();
    };

    std::vector<size_t> all_indices;
    all_indices.reserve(regions.size());
    for (size_t index = 0; index < regions.size(); ++index)
        all_indices.push_back(index);

    // A single fully parallel pass makes every location visible quickly.
    // Only nearby candidates receive extra probes, avoiding the old full
    // five-probe sweep across every advertised region.
    probe_indices(all_indices, 1, kCoarseTimeoutMs);
    for (size_t index = 0; index < regions.size(); ++index)
    {
        const auto estimate = server_location::EstimateLatency(samples[index], 1);
        regions[index].ping_ms = estimate.median_ms;
        regions[index].jitter_ms = estimate.jitter_ms;
        regions[index].selection_score_ms = estimate.selection_score_ms;
        regions[index].successful_probes = estimate.successful_probes;
        regions[index].attempted_probes = 1;
    }

    std::vector<size_t> finalists = server_location::SelectLatencyFinalists(
        regions, kMaximumFinalists, kFinalistMarginMs);
    if (finalists.empty())
    {
        // Retry only when the short pass produced no usable route at all.
        probe_indices(all_indices, 1, kRetryTimeoutMs);
        for (size_t index = 0; index < regions.size(); ++index)
        {
            const auto estimate = server_location::EstimateLatency(samples[index], 2);
            regions[index].ping_ms = estimate.median_ms;
            regions[index].jitter_ms = estimate.jitter_ms;
            regions[index].selection_score_ms = estimate.selection_score_ms;
            regions[index].successful_probes = estimate.successful_probes;
            regions[index].attempted_probes = 2;
        }
        finalists = server_location::SelectLatencyFinalists(
            regions, kMaximumFinalists, kFinalistMarginMs);
    }

    probe_indices(finalists, kRefineProbeCount, kRefineTimeoutMs);
    const std::unordered_set<size_t> finalist_set(
        finalists.begin(), finalists.end());
    for (size_t index = 0; index < regions.size(); ++index)
    {
        if (finalist_set.count(index) != 0)
        {
            const int attempted = regions[index].attempted_probes +
                kRefineProbeCount;
            const auto estimate =
                server_location::EstimateLatency(samples[index], attempted);
            regions[index].ping_ms = estimate.median_ms;
            regions[index].jitter_ms = estimate.jitter_ms;
            regions[index].selection_score_ms = estimate.selection_score_ms;
            regions[index].successful_probes = estimate.successful_probes;
            regions[index].attempted_probes = attempted;
        }
        else if (regions[index].ping_ms >= 0)
        {
            // Coarse-only locations stay visible in Settings, but cannot beat
            // a finalist based on one lucky sample.
            regions[index].selection_score_ms =
                regions[index].ping_ms + kNonFinalistPenaltyMs;
        }
    }
    return regions;
}

std::vector<StreamRegion> GfnClient::LoadCachedStreamRegions(
    const AuthSession& session) const
{
    const std::string provider_url =
        server_location::NormalizeStreamingBaseUrl(
            session.provider.streaming_service_url);
    std::lock_guard<std::mutex> lock(g_region_selection_cache.mutex);
    const PersistedRegionSelection cache = LoadPersistentRegionSelection(
        provider_url, kRegionDisplayCacheLifetimeMs);
    return cache.valid ? cache.regions : std::vector<StreamRegion> {};
}

void GfnClient::SaveStreamRegionMeasurements(
    const AuthSession& session,
    const std::vector<StreamRegion>& regions) const
{
    const std::string provider_url =
        server_location::NormalizeStreamingBaseUrl(
            session.provider.streaming_service_url);
    const std::string selected_url =
        server_location::SelectBestMeasuredRegionUrl(regions);
    if (provider_url.empty() || selected_url.empty())
        return;

    std::string selected_name;
    const auto selected = std::find_if(
        regions.begin(), regions.end(), [&selected_url](const StreamRegion& region) {
            return server_location::NormalizeStreamingBaseUrl(region.url) ==
                selected_url;
        });
    if (selected != regions.end())
        selected_name = selected->name;

    std::lock_guard<std::mutex> lock(g_region_selection_cache.mutex);
    g_region_selection_cache.provider_url = provider_url;
    g_region_selection_cache.selected_url = selected_url;
    g_region_selection_cache.selected_name = selected_name;
    g_region_selection_cache.regions = regions;
    g_region_selection_cache.measured_at = std::chrono::steady_clock::now();
    g_region_selection_cache.provider_fallback = false;
    try
    {
        SavePersistentRegionSelection(
            provider_url, selected_url, selected_name, regions);
    }
    catch (const std::exception& e)
    {
        AppendSessionTraceLog(
            "REGION cache write failed error=" + std::string(e.what()));
    }
}

std::vector<LoginProvider> GfnClient::FetchLoginProviders() const
{
    const HttpResponse response = http_client_.Get(
        kServiceUrlsEndpoint,
        kUserAgent,
        {"Accept: application/json"});

    if (response.status_code != 200)
    {
        throw std::runtime_error(
            "Provider discovery failed with HTTP " + std::to_string(response.status_code));
    }

    JsonPtr root = LoadJson(response.body);
    json_t* gfn_service_info =
        json_object_get(root.get(), "gfnServiceInfo");
    json_t* endpoints =
        gfn_service_info ? json_object_get(gfn_service_info, "gfnServiceEndpoints") : nullptr;

    std::vector<LoginProvider> providers;
    if (json_is_array(endpoints))
    {
        size_t index = 0;
        json_t* entry = nullptr;
        json_array_foreach(endpoints, index, entry)
        {
            LoginProvider provider;
            provider.idp_id                = GetString(entry, "idpId");
            provider.code                  = GetString(entry, "loginProviderCode");
            provider.display_name          = GetString(entry, "loginProviderDisplayName");
            provider.streaming_service_url = EnsureTrailingSlash(GetString(entry, "streamingServiceUrl"));
            provider.priority              = GetInteger(entry, "loginProviderPriority", 0);

            if (provider.code == "BPC")
                provider.display_name = "bro.game";

            if (!provider.idp_id.empty() && !provider.streaming_service_url.empty())
                providers.push_back(provider);
        }
    }

    if (providers.empty())
        providers.push_back(DefaultProvider());

    std::sort(
        providers.begin(),
        providers.end(),
        [](const LoginProvider& left, const LoginProvider& right) {
            return left.priority < right.priority;
        });

    return providers;
}

std::vector<PublicGame> GfnClient::FetchPublicGames() const
{
    const HttpResponse response = http_client_.Get(
        kPublicCatalogEndpoint,
        kUserAgent,
        {"Accept: application/json"});

    if (response.status_code != 200)
    {
        throw std::runtime_error(
            "Public catalog fetch failed with HTTP " + std::to_string(response.status_code));
    }

    JsonPtr root = LoadJson(response.body);
    if (!json_is_array(root.get()))
        throw std::runtime_error("Public catalog payload is not a JSON array");

    std::vector<PublicGame> games;

    size_t index = 0;
    json_t* item = nullptr;
    json_array_foreach(root.get(), index, item)
    {
        const std::string status = GetString(item, "status");
        const std::string title  = GetString(item, "title");
        if (status != "AVAILABLE" || title.empty())
            continue;

        PublicGame game;
        game.id        = GetString(item, "id");
        game.title     = title;
        game.store     = InferStore(item);
        game.publisher = GetString(item, "publisher");
        game.developer = GetString(item, "developer");
        game.genres = ParseLabelArray(json_object_get(item, "genres"));
        game.image_url = BuildSteamImageUrl(GetString(item, "steamUrl"));
        game.poster_url = game.image_url;
        game.description = GetString(item, "description");
        if (game.description.empty())
            game.description = GetString(item, "longDescription");
        if (game.id.empty())
            game.id = game.title;

        games.push_back(std::move(game));
    }

    std::sort(
        games.begin(),
        games.end(),
        [](const PublicGame& left, const PublicGame& right) {
            return left.title < right.title;
        });

    return games;
}

std::vector<PublicGame> GfnClient::FetchCatalogGames(
    AuthSession& session, const std::string& search_query) const
{
    session = RecoverSavedSession(session);
    std::string jwt_token = ResolveSessionJwt(session);
    const std::string vpc_id = ResolveVpcId(http_client_, session);

    std::vector<PublicGame> games;
    std::unordered_set<std::string> seen_ids;
    std::string cursor;

    for (int page = 0; page < 3; ++page)
    {
        HttpResponse response = http_client_.Post(
            kGraphQlEndpoint,
            kUserAgent,
            BuildGraphQlPostHeaders(jwt_token),
            BuildCatalogRequestBody(vpc_id, search_query, cursor));

        if (response.status_code == 401)
        {
            session = RecoverSavedSession(session, true);
            jwt_token = ResolveSessionJwt(session);
            response = http_client_.Post(
                kGraphQlEndpoint,
                kUserAgent,
                BuildGraphQlPostHeaders(jwt_token),
                BuildCatalogRequestBody(vpc_id, search_query, cursor));
        }

        if (response.status_code != 200)
        {
            throw std::runtime_error(
                "Catalog browse failed with HTTP " + std::to_string(response.status_code));
        }

        JsonPtr root = LoadJson(response.body);
        bool has_next_page = false;
        std::string end_cursor;
        std::vector<PublicGame> page_games =
            ParseCatalogPage(root, has_next_page, end_cursor);
        for (PublicGame& game : page_games)
        {
            const std::string key = game.id.empty() ? game.title : game.id;
            if (seen_ids.insert(key).second)
                games.push_back(std::move(game));
        }

        if (!has_next_page || end_cursor.empty() || end_cursor == cursor)
            break;
        cursor = std::move(end_cursor);
    }

    return games;
}

AuthSession GfnClient::Login(const LoginProvider& provider, const std::string& login_hint) const
{
    AppendAuthLog("auth: login started provider=" + provider.code);

    const PkcePair pkce = GeneratePkce();
    const std::string state = HexEncode(GenerateRandomBytes(32).data(), 32);
    const std::string auth_url = BuildAuthorizeUrl(provider, pkce, state, kRedirectUri, login_hint);
    const std::string final_url = RunBrowserLogin(auth_url);

    if (GetQueryValue(final_url, "state") != state)
        throw std::runtime_error("GeForce NOW login callback failed the OAuth state check");

    const std::string error = GetQueryValue(final_url, "error");
    if (!error.empty())
    {
        throw std::runtime_error("GeForce NOW login failed: " + error);
    }

    const std::string code = GetQueryValue(final_url, "code");
    if (code.empty())
        throw std::runtime_error("The GeForce NOW login callback did not include an authorization code");

    NativeAuthHttpSession token_http;
    WarmNvidiaTokenConnection(token_http);
    AuthTokens tokens = ExchangeAuthorizationCode(token_http, code, pkce.verifier);
    AppendAuthLog("auth: token exchange ok");
    RequestClientToken(token_http, tokens);

    AuthSession session;
    session.provider = provider;
    session.tokens   = tokens;
    session.user     = FetchUserInfo(token_http, tokens);
    session.last_refresh_at_ms = NowMs();
    RefreshMembershipTier(http_client_, session);
    AppendAuthLog("auth: user info ok user_id_present=" + std::string(session.user.user_id.empty() ? "no" : "yes"));
    return session;
}
