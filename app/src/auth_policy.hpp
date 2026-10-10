#pragma once

#include <cstdint>

namespace opennow::auth
{

enum class RefreshMechanism
{
    None,
    ClientToken,
    OAuthRefreshToken,
};

constexpr RefreshMechanism SelectRefreshMechanism(
    bool has_client_token, bool has_user_id, bool has_refresh_token)
{
    if (has_client_token && has_user_id)
        return RefreshMechanism::ClientToken;
    if (has_refresh_token)
        return RefreshMechanism::OAuthRefreshToken;
    return RefreshMechanism::None;
}

constexpr bool ShouldRefresh(std::int64_t expires_at_ms, std::int64_t now_ms,
                             std::int64_t refresh_window_ms)
{
    return expires_at_ms <= 0 || expires_at_ms - now_ms < refresh_window_ms;
}

constexpr bool IsExpired(std::int64_t expires_at_ms, std::int64_t now_ms)
{
    return expires_at_ms > 0 && expires_at_ms <= now_ms;
}

constexpr bool IsTemporaryHttpStatus(int status)
{
    return status == 0 || status == 408 || status == 429 || status >= 500;
}

constexpr int RefreshRetryDelayMs(int completed_attempts)
{
    return completed_attempts <= 1 ? 500 : 1500;
}

} // namespace opennow::auth
