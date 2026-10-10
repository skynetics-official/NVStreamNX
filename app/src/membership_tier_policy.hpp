#pragma once

#include <algorithm>
#include <cctype>
#include <string>

namespace opennow::membership
{

enum class TierAccent
{
    Neutral,
    Performance,
    Ultimate,
};

inline std::string NormalizeTier(std::string tier)
{
    std::transform(tier.begin(), tier.end(), tier.begin(), [](unsigned char value) {
        return static_cast<char>(std::toupper(value));
    });
    return tier;
}

inline bool IsVerifiedFreeTier(const std::string& tier, bool verified)
{
    return verified && NormalizeTier(tier) == "FREE";
}

inline TierAccent AccentForTier(const std::string& tier)
{
    const std::string normalized = NormalizeTier(tier);
    if (normalized == "ULTIMATE")
        return TierAccent::Ultimate;
    if (normalized == "PERFORMANCE" || normalized == "PRIORITY")
        return TierAccent::Performance;
    return TierAccent::Neutral;
}

inline std::string DisplayLabel(const std::string& tier, bool verified)
{
    return verified && !tier.empty() ? tier : "Unknown";
}

} // namespace opennow::membership
