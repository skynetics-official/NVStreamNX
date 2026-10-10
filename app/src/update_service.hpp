#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace opennow::update
{

struct ReleaseInfo
{
    std::string version;
    std::string asset_name;
    std::string download_url;
    std::string checksum_url;
    std::string sha256;
    std::uint64_t size = 0;
};

struct CheckResult
{
    bool ok = false;
    bool update_available = false;
    ReleaseInfo release;
    std::string error;
};

using ProgressCallback = std::function<void(std::uint64_t, std::uint64_t)>;

CheckResult CheckLatestRelease(const std::string& current_version);
bool StageRelease(
    const ReleaseInfo& release,
    const ProgressCallback& progress,
    std::string& error);
bool LaunchUpdateHelper(std::string& error);
void ConfirmInstalledUpdate();

inline constexpr const char* kCanonicalNroPath =
    "sdmc:/switch/NVStreamNX/NVStreamNX.nro";

} // namespace opennow::update
