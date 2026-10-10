#ifdef __SWITCH__
#include <switch.h>
#include <sys/stat.h>
#endif

#include <curl/curl.h>

#include <borealis.hpp>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "main_activity.hpp"
#include "app_version.hpp"
#include "app_paths.hpp"
#include "cover_image_cache.hpp"
#include "home_shortcut.hpp"
#include "localization.hpp"
#include "runtime_journal.hpp"
#include "stream_diagnostics.hpp"
#include "stream_settings.hpp"
#include "ui_helpers.hpp"
#include "update_service.hpp"

#ifdef __SWITCH__
namespace
{

void EnsureLogDirectory()
{
    opennow::PrepareAppStorage();
}

void AppendBootLog(const std::string& line)
{
    EnsureLogDirectory();

    std::ofstream stream(opennow::AppHomePath() + "/boot.log", std::ios::app);
    if (!stream.is_open())
        return;

    stream << line << '\n';
}

void ShowStartupFailure(const std::string& message)
{
    AppendBootLog("startup failure: " + message);

    consoleInit(nullptr);
    consoleClear();
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);

    printf("NVStreamNX failed to start.\n\n");
    printf("%s\n\n", message.c_str());
    printf("Diagnostics were written to:\n");
    printf("sdmc:/switch/NVStreamNX/boot.log\n");
    printf("sdmc:/switch/NVStreamNX/runtime.log\n\n");
    printf("Press PLUS or B to exit.\n");

    while (appletMainLoop())
    {
        padUpdate(&pad);
        const u64 down = padGetButtonsDown(&pad);
        if (down & (HidNpadButton_Plus | HidNpadButton_B))
            break;

        consoleUpdate(nullptr);
    }

    consoleExit(nullptr);
}

} // namespace
#endif

namespace
{

class ShortcutResultView final : public brls::Box
{
  public:
    ShortcutResultView(
        opennow::shortcut::InstallerResult result,
        std::function<void()> continue_to_application)
        : brls::Box(brls::Axis::COLUMN)
        , continue_to_application_(std::move(continue_to_application))
    {
        setGrow(1.0f);
        setAlignItems(brls::AlignItems::CENTER);
        setJustifyContent(brls::JustifyContent::CENTER);
        setBackgroundColor(nvgRGB(10, 12, 15));

        auto* card = new brls::Box(brls::Axis::COLUMN);
        card->setWidth(690);
        card->setPadding(34, 42, 34, 42);
        card->setAlignItems(brls::AlignItems::CENTER);
        card->setBackgroundColor(nvgRGB(22, 25, 30));
        card->setCornerRadius(18);

        auto* eyebrow = new brls::Label();
        eyebrow->setText("NVStreamNX");
        eyebrow->setFontSize(14);
        eyebrow->setTextColor(nvgRGB(77, 218, 130));
        eyebrow->setHorizontalAlign(brls::HorizontalAlign::CENTER);
        eyebrow->setMarginBottom(14);
        card->addView(eyebrow);

        const std::string shortcut_title = result.title.empty()
            ? opennow::Tr("Game shortcut")
            : result.title;
        auto* heading = new brls::Label();
        heading->setText(result.success
                ? opennow::Tr("HOME shortcut created")
                : opennow::Tr("HOME shortcut installation failed"));
        heading->setFontSize(29);
        heading->setTextColor(result.success
                ? nvgRGB(242, 246, 248)
                : nvgRGB(255, 132, 132));
        heading->setHorizontalAlign(brls::HorizontalAlign::CENTER);
        heading->setMarginBottom(18);
        card->addView(heading);

        auto* message = new brls::Label();
        message->setWidth(600);
        message->setText(result.success
                ? opennow::TrFormat(
                      "The shortcut for {0} was created successfully and added to the HOME Menu.",
                      {shortcut_title})
                : (result.error.empty()
                          ? opennow::Tr("HOME shortcut installation failed.")
                          : result.error));
        message->setFontSize(20);
        message->setTextColor(nvgRGB(188, 195, 204));
        message->setHorizontalAlign(brls::HorizontalAlign::CENTER);
        message->setMarginBottom(28);
        card->addView(message);

        close_button_ = new brls::Button();
        close_button_->setStyle(&brls::BUTTONSTYLE_BORDERED);
        close_button_->setWidth(600);
        close_button_->setText(opennow::Tr("Close"));
        close_button_->registerClickAction([this](brls::View*) {
            if (continued_)
                return true;
            continued_ = true;
            opennow::LogRuntimeEvent(
                "shortcut", "result_screen_close", "continue_to_application");
            auto continuation =
                std::make_shared<std::function<void()>>(
                    std::move(continue_to_application_));
            if (!brls::Application::popActivity(
                    brls::TransitionAnimation::FADE,
                    [continuation]() mutable {
                        // Borealis invokes the pop callback before deleting the
                        // old activity and unblocking input. Build the main UI
                        // on the next UI cycle, after teardown is complete.
                        brls::sync([continuation]() mutable {
                            if (*continuation)
                            {
                                auto callback = std::move(*continuation);
                                callback();
                            }
                        });
                    }))
            {
                continued_ = false;
                continue_to_application_ = std::move(*continuation);
            }
            return true;
        });
        card->addView(close_button_);
        card->setLastFocusedView(close_button_);
        addView(card);
        setLastFocusedView(close_button_);

        opennow::LogRuntimeEvent(
            "shortcut", "result_screen_open",
            result.success ? "success title=" + shortcut_title
                           : "failed error=" + result.error);
    }

    brls::View* getDefaultFocus() override
    {
        return close_button_;
    }

  private:
    brls::Button* close_button_ = nullptr;
    std::function<void()> continue_to_application_;
    bool continued_ = false;
};

} // namespace

int main(int argc, char* argv[])
{
    opennow::PrepareAppStorage();
    if (argc > 0 && argv && argv[0])
        opennow::shortcut::SetExecutablePath(argv[0]);
    auto launch_request = opennow::shortcut::ReadLaunchRequest(argc, argv);
    auto installer_result = opennow::shortcut::ConsumeInstallerResult();
    opennow::InitializeRuntimeJournal(opennow::kAppVersion);
    const auto shortcut_repair =
        opennow::shortcut::RepairLegacyShortcutManifests();
    opennow::LogRuntimeEvent(
        "shortcut", "manifest_repair",
        "scanned=" + std::to_string(shortcut_repair.scanned) +
            " repaired=" + std::to_string(shortcut_repair.repaired) +
            " failed=" + std::to_string(shortcut_repair.failed));
    AppendBootLog("boot: entered main()");

#ifdef __SWITCH__
    appletSetWirelessPriorityMode(AppletWirelessPriorityMode_OptimizedForWlan);
#endif

    brls::Logger::setLogLevel(brls::LogLevel::LOG_DEBUG);

    try
    {
        AppendBootLog("boot: calling curl_global_init()");

        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
        {
            ShowStartupFailure("curl_global_init failed");
            return EXIT_FAILURE;
        }

        AppendBootLog("boot: curl_global_init ok");
        if (const curl_version_info_data* version =
                curl_version_info(CURLVERSION_NOW))
        {
            opennow::LogRuntimeEvent(
                "tls", "backend",
                "curl=" +
                    std::string(version->version ? version->version : "unknown") +
                    " ssl=" +
                    std::string(
                        version->ssl_version ? version->ssl_version : "unknown"));
        }
        AppendBootLog("boot: calling brls::Application::init()");

        if (!brls::Application::init())
        {
            ShowStartupFailure("brls::Application::init returned false");
            curl_global_cleanup();
            return EXIT_FAILURE;
        }

        AppendBootLog("boot: brls::Application::init ok");
        AppendBootLog("boot: creating window");

        const opennow::StreamSettings startup_settings = opennow::LoadStreamSettings();
        opennow::SetInterfaceLanguage(startup_settings.interface_language);
        brls::Application::createWindow("NVStreamNX");
        AppendBootLog("boot: window created");

        // Plus is an in-game Xbox Start/Guide input. Borealis otherwise binds
        // BUTTON_START globally to Application::quit() before StreamView sees it.
        brls::Application::setGlobalQuit(false);
        brls::Application::setFPSStatus(false);
        opennow::SetStreamDiagnosticsEnabled(
            startup_settings.debug_diagnostics);
        opennow::LogRuntimeEvent(
            "diagnostics", "mode",
            startup_settings.debug_diagnostics
                ? "extended=enabled runtime=always_on"
                : "extended=disabled runtime=always_on");

        auto launch_request_holder =
            std::make_shared<std::optional<opennow::shortcut::LaunchRequest>>(
                std::move(launch_request));
        auto open_main_activity = [launch_request_holder]() mutable {
            std::optional<opennow::shortcut::LaunchRequest> request;
            if (*launch_request_holder)
                request = std::move(**launch_request_holder);
            launch_request_holder->reset();
            AppendBootLog("boot: pushing main activity");
            brls::Application::pushActivity(
                new opennow::MainActivity(std::move(request)),
                brls::TransitionAnimation::FADE);
            AppendBootLog("boot: main activity pushed");
        };

        if (installer_result)
        {
            // Borealis deliberately refuses to pop the first activity. Keep an
            // inert root below the installer result so Close can remove the
            // result screen before constructing the network-backed main UI.
            auto* bootstrap_root = new brls::Box(brls::Axis::COLUMN);
            bootstrap_root->setGrow(1.0f);
            bootstrap_root->setBackgroundColor(nvgRGB(10, 12, 15));
            brls::Application::pushActivity(
                new brls::Activity(bootstrap_root),
                brls::TransitionAnimation::NONE);
            AppendBootLog("boot: shortcut bootstrap activity pushed");

            AppendBootLog("boot: pushing shortcut result activity");
            brls::Application::pushActivity(
                new brls::Activity(new ShortcutResultView(
                    std::move(*installer_result), open_main_activity)),
                brls::TransitionAnimation::NONE);
            AppendBootLog("boot: shortcut result activity pushed");
        }
        else
        {
            open_main_activity();
        }
        opennow::update::ConfirmInstalledUpdate();
        while (brls::Application::mainLoop())
        {
        }

        AppendBootLog("boot: main loop exited");
        opennow::ShutdownImageCache();
        opennow::ShutdownRuntimeJournal();
        curl_global_cleanup();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex)
    {
        opennow::LogRuntimeEvent("app", "unhandled_exception", ex.what());
        ShowStartupFailure(std::string("Unhandled exception: ") + ex.what());
    }
    catch (...)
    {
        opennow::LogRuntimeEvent("app", "unhandled_exception", "unknown");
        ShowStartupFailure("Unhandled non-standard exception");
    }

    opennow::ShutdownImageCache();
    opennow::ShutdownRuntimeJournal();
    curl_global_cleanup();
    return EXIT_FAILURE;
}
