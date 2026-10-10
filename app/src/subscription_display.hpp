#pragma once

#include "models.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace opennow::subscription
{

inline std::string FormatDecimal(double value)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(
        std::fabs(value - std::round(value)) < 0.01 ? 0 : 1) << value;
    return stream.str();
}

inline std::string FormatTimeRemaining(const SubscriptionInfo& info)
{
    if (!info.available)
        return "Unknown";
    if (info.is_unlimited)
        return "Unlimited";

    const int minutes = std::max(0, static_cast<int>(std::round(info.remaining_hours * 60.0)));
    if (minutes < 60)
        return std::to_string(minutes) + "m left";
    const int hours = minutes / 60;
    const int remainder = minutes % 60;
    return remainder == 0
        ? std::to_string(hours) + "h left"
        : std::to_string(hours) + "h " + std::to_string(remainder) + "m left";
}

inline std::string FormatStorageRemaining(const SubscriptionInfo& info)
{
    if (!info.available)
        return "Unknown";
    if (!info.has_storage)
        return "No storage";
    if (!info.has_storage_usage)
        return FormatDecimal(info.storage_size_gb) + " GB total";
    return FormatDecimal(std::max(0.0, info.storage_size_gb - info.storage_used_gb)) + " GB left";
}

inline std::string FormatStorageUsage(const SubscriptionInfo& info)
{
    if (!info.available)
        return "Unknown";
    if (!info.has_storage)
        return "Not included";
    if (!info.has_storage_usage)
        return FormatDecimal(info.storage_size_gb) + " GB total";
    return FormatDecimal(info.storage_used_gb) + " GB used of " +
        FormatDecimal(info.storage_size_gb) + " GB";
}

} // namespace opennow::subscription
