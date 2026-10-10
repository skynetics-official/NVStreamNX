#pragma once

#include "models.hpp"

#include <algorithm>
#include <string>

namespace opennow::ad_report
{

inline SessionAdReport Started(const std::string& ad_id)
{
    SessionAdReport report;
    report.ad_id = ad_id;
    report.action = SessionAdAction::Start;
    report.paused_time_ms = 0;
    return report;
}

inline SessionAdReport Finished(
    const std::string& ad_id, int watched_time_ms,
    int expected_duration_ms = 0)
{
    SessionAdReport report;
    report.ad_id = ad_id;
    report.action = SessionAdAction::Finish;
    // Match the browser implementation: report the observed playback clock,
    // not an inflated declared duration. Clamp small container overruns to the
    // advertised length because CloudMatch rejects impossible watch times.
    report.watched_time_ms = std::max(0, watched_time_ms);
    if (expected_duration_ms > 0)
        report.watched_time_ms = std::min(
            report.watched_time_ms, expected_duration_ms);
    report.paused_time_ms = 0;
    return report;
}

inline bool IsRecoverableTransitionError(const std::string& error)
{
    return error.find("INVALID_REQUEST_STATUS") != std::string::npos ||
           error.find("statusCode=5") != std::string::npos;
}

inline SessionAdReport Cancelled(
    const std::string& ad_id, int watched_time_ms, std::string reason)
{
    SessionAdReport report;
    report.ad_id = ad_id;
    report.action = SessionAdAction::Cancel;
    report.watched_time_ms = std::max(0, watched_time_ms);
    report.paused_time_ms = 0;
    report.cancel_reason = std::move(reason);
    return report;
}

} // namespace opennow::ad_report
