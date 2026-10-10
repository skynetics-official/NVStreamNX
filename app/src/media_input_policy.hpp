#pragma once

#include <string>
#include <string_view>

namespace opennow::media_input
{

inline bool HasScheme(std::string_view value)
{
    const auto separator = value.find(':');
    if (separator == std::string_view::npos || separator == 0)
        return false;

    for (size_t index = 0; index < separator; ++index)
    {
        const char ch = value[index];
        const bool valid = (ch >= 'a' && ch <= 'z') ||
                           (ch >= 'A' && ch <= 'Z') ||
                           (index > 0 && ch >= '0' && ch <= '9') ||
                           (index > 0 && (ch == '+' || ch == '-' || ch == '.'));
        if (!valid)
            return false;
    }
    return true;
}

inline std::string FfmpegLocalFileUrl(std::string_view path)
{
    if (path.rfind("file:", 0) == 0)
        return std::string(path);

    // libnx mount paths contain a colon (sdmc:/...), which FFmpeg otherwise
    // interprets as an unsupported network protocol named "sdmc".
    return "file:" + std::string(path);
}

} // namespace opennow::media_input
