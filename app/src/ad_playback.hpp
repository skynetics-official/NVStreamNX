#pragma once

#include "http_client.hpp"
#include "models.hpp"

#include <borealis.hpp>

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

extern "C" {
#include <libavutil/frame.h>
}

namespace opennow
{

struct AdvertisementPlaybackResult
{
    bool started = false;
    bool completed = false;
    bool cancelled = false;
    int watched_time_ms = 0;
    std::string error;
};

class AdvertisementPlaybackState
{
  public:
    AdvertisementPlaybackState();
    ~AdvertisementPlaybackState();

    AdvertisementPlaybackState(const AdvertisementPlaybackState&) = delete;
    AdvertisementPlaybackState& operator=(const AdvertisementPlaybackState&) = delete;

    void Begin(std::string title, int duration_seconds);
    void PresentFrame(const AVFrame* frame, int watched_time_ms);
    void SetProgress(int watched_time_ms, std::string status);
    void Finish(std::string status);

  private:
    friend class AdvertisementPlaybackView;
    mutable std::mutex mutex_;
    AVFrame* frame_ = nullptr;
    uint64_t generation_ = 0;
    int duration_ms_ = 0;
    int watched_ms_ = 0;
    bool finished_ = false;
    std::string title_;
    std::string status_;
};

class AdvertisementPlaybackView final : public brls::View
{
  public:
    explicit AdvertisementPlaybackView(
        std::shared_ptr<AdvertisementPlaybackState> state);
    ~AdvertisementPlaybackView() override;

    void draw(NVGcontext* vg, float x, float y, float width, float height,
              brls::Style style, brls::FrameContext* ctx) override;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

AdvertisementPlaybackResult PlayAdvertisementMedia(
    const SessionAdInfo& ad,
    const AdMediaFileInfo& media,
    const HttpClient& http_client,
    const std::shared_ptr<AdvertisementPlaybackState>& state,
    const std::function<bool()>& is_cancelled,
    const std::function<void()>& on_started);

} // namespace opennow
