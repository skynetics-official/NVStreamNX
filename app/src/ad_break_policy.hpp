#pragma once

#include "models.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace opennow::ad_break
{

enum class PresentationState
{
    None,
    AwaitingMetadata,
    MediaReady,
};

inline PresentationState Classify(const AdBreakInfo& value)
{
    if (!value.required)
        return PresentationState::None;
    return value.ads.empty() ? PresentationState::AwaitingMetadata
                             : PresentationState::MediaReady;
}

inline int TotalDurationSeconds(const AdBreakInfo& value)
{
    int total = 0;
    for (const auto& ad : value.ads)
        total += std::max(0, ad.duration_seconds);
    return total;
}

inline std::string Lowercase(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

inline int MediaPreferenceScore(const AdMediaFileInfo& media)
{
    const std::string value = Lowercase(media.encoding_profile + " " + media.url);
    if (value.find("mp4") != std::string::npos)
        return 300;
    if (value.find("hls") != std::string::npos || value.find("m3u8") != std::string::npos)
        return 200;
    if (value.find("webm") != std::string::npos)
        return 100;
    return media.url.empty() ? 0 : 10;
}

inline const AdMediaFileInfo* SelectPreferredMedia(const AdBreakInfo& value)
{
    const AdMediaFileInfo* best = nullptr;
    int best_score = -1;
    for (const auto& ad : value.ads)
    {
        for (const auto& media : ad.media_files)
        {
            const int score = MediaPreferenceScore(media);
            if (score > best_score)
            {
                best = &media;
                best_score = score;
            }
        }
    }
    return best;
}

inline const AdMediaFileInfo* SelectPreferredMedia(const SessionAdInfo& value)
{
    const AdMediaFileInfo* best = nullptr;
    int best_score = -1;
    for (const auto& media : value.media_files)
    {
        const int score = MediaPreferenceScore(media);
        if (score > best_score)
        {
            best = &media;
            best_score = score;
        }
    }
    return best;
}

inline bool IsWaitingAdsTimeout(int status_code, std::string_view description)
{
    return status_code == 79 ||
           description.find("SESSION_WAITING_ADS_TIME_EXPIRED") != std::string_view::npos;
}

} // namespace opennow::ad_break
