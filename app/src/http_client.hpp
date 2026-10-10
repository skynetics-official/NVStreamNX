#pragma once

#include <string>
#include <vector>
#include <functional>

namespace opennow
{

struct HttpResponse
{
    long status_code = 0;
    std::string body;
};

class HttpClient
{
  public:
    HttpResponse Request(
        const std::string& method,
        const std::string& url,
        const std::string& user_agent,
        const std::vector<std::string>& headers = {},
        const std::string& body = {},
        long connect_timeout_seconds = 10,
        long timeout_seconds = 30) const;

    HttpResponse Get(
        const std::string& url,
        const std::string& user_agent,
        const std::vector<std::string>& headers = {},
        long connect_timeout_seconds = 10,
        long timeout_seconds = 30) const;

    HttpResponse Post(
        const std::string& url,
        const std::string& user_agent,
        const std::vector<std::string>& headers,
        const std::string& body) const;

    void DownloadToFile(
        const std::string& url,
        const std::string& destination,
        const std::string& user_agent,
        const std::vector<std::string>& headers = {},
        const std::function<void(long long, long long)>& on_progress = {},
        const std::function<bool()>& is_cancelled = {}) const;

    int MeasureTcpHandshakeLatencyMs(
        const std::string& url,
        long timeout_ms = 3000) const noexcept;
};

} // namespace opennow
