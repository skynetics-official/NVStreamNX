#pragma once

#include <array>
#include <cctype>
#include <cstdint>
#include <string>

namespace opennow::update
{

struct Version
{
    std::array<std::uint64_t, 3> core {};
    std::uint64_t revision = 0;
};

inline bool ParseVersion(std::string text, Version& version)
{
    version = {};
    if (!text.empty() && (text.front() == 'v' || text.front() == 'V'))
        text.erase(text.begin());

    const std::size_t suffix = text.find("-r");
    std::string revision;
    if (suffix != std::string::npos)
    {
        revision = text.substr(suffix + 2);
        text.resize(suffix);
        if (revision.empty())
            return false;
    }

    std::size_t start = 0;
    for (std::size_t index = 0; index < version.core.size(); ++index)
    {
        const std::size_t end = text.find('.', start);
        if ((index + 1 == version.core.size()) != (end == std::string::npos))
            return false;
        const std::string part = text.substr(start, end - start);
        if (part.empty())
            return false;
        for (const unsigned char character : part)
            if (!std::isdigit(character))
                return false;
        try
        {
            version.core[index] = std::stoull(part);
        }
        catch (...)
        {
            return false;
        }
        start = end + 1;
    }

    if (!revision.empty())
    {
        for (const unsigned char character : revision)
            if (!std::isdigit(character))
                return false;
        try
        {
            version.revision = std::stoull(revision);
        }
        catch (...)
        {
            return false;
        }
    }
    return true;
}

inline bool IsNewerVersion(const std::string& candidate, const std::string& current)
{
    Version left;
    Version right;
    if (!ParseVersion(candidate, left) || !ParseVersion(current, right))
        return false;
    if (left.core != right.core)
        return left.core > right.core;
    return left.revision > right.revision;
}

inline bool IsTrustedReleaseAsset(const std::string& url)
{
    static const std::string prefix =
        "https://github.com/skynetics-official/NVStreamNX/releases/download/";
    return url.rfind(prefix, 0) == 0;
}

} // namespace opennow::update
