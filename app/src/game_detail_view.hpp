#pragma once

#include "gfn_client.hpp"
#include "models.hpp"

#include <borealis.hpp>

#include <atomic>
#include <memory>
#include <string>
#include <vector>

namespace opennow
{

struct GameDetailData
{
    std::string title;
    std::string game_id;
    std::string subtitle;
    std::string image_url;
    std::string poster_url;
    std::string hero_url;
    std::vector<std::string> screenshots;
    std::string launch_app_id;
    std::string publisher;
    std::string developer;
    std::string description;
    std::string stores;
    std::string membership_tier_label;
    std::string last_played;
    std::vector<std::string> genres;
    std::vector<std::string> feature_labels;
    std::vector<std::string> content_ratings;
    std::vector<std::string> supported_controls;
    bool owned = false;
    std::vector<GameVariant> variants;
};

class GameDetailView : public brls::Box
{
  public:
    GameDetailView(const GfnClient& client, GameDetailData data);
    ~GameDetailView() override;

  private:
    void Play();
    void ShowStoreSelector(bool launch_after_selection);
    void LaunchSelectedVariant();
    void CreateSwitchShortcut();
    void UpdateStoreButton();
    void OpenNteCredentialsMenu();
    void ConfigureNteCredentials();
    void UpdateNteButton();
    void ShowPreviousScreenshot();
    void ShowNextScreenshot();
    void UpdateScreenshot();
    std::string ActiveUserId() const;

    GfnClient client_;
    GameDetailData data_;
    brls::Button* store_button_ = nullptr;
    brls::Button* nte_button_ = nullptr;
    brls::Image* screenshot_image_ = nullptr;
    std::vector<brls::Label*> screenshot_dots_;
    std::vector<std::string> media_urls_;
    std::shared_ptr<std::atomic_uint64_t> screenshot_generation_ =
        std::make_shared<std::atomic_uint64_t>(0);
    std::shared_ptr<std::atomic_bool> alive_ =
        std::make_shared<std::atomic_bool>(true);
    size_t screenshot_index_ = 0;
    size_t selected_variant_index_ = 0;
    bool launcher_preference_loaded_ = false;
};

GameDetailData MakeLibraryGameDetail(const GameInfo& game);
GameDetailData MakeCatalogGameDetail(const PublicGame& game);

} // namespace opennow
