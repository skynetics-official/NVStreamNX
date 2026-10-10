#include "game_detail_view.hpp"

#include "app_state.hpp"
#include "cover_image_cache.hpp"
#include "description_scroll_policy.hpp"
#include "home_shortcut.hpp"
#include "nte_credentials.hpp"
#include "ui_helpers.hpp"
#include "localization.hpp"

#ifdef __SWITCH__
#include <switch.h>
#endif

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <sstream>

namespace opennow
{
namespace
{

class DescriptionScrollingFrame final : public brls::ScrollingFrame
{
  public:
    DescriptionScrollingFrame(
        brls::View* return_focus, brls::Label* focus_hint)
        : return_focus_(return_focus)
        , focus_hint_(focus_hint)
    {
        // Input is handled explicitly below. CENTERED prevents Borealis'
        // natural mode from changing focus while an Up/Down press is also
        // changing the description offset.
        setScrollingBehavior(brls::ScrollingBehavior::CENTERED);
        setHideHighlightBackground(true);
        setHideHighlightBorder(true);
        setBorderThickness(0.0f);
        setBorderColor(nvgRGB(65, 229, 133));

        registerAction(
            "", brls::BUTTON_NAV_UP,
            [this](brls::View*) {
                return Scroll(DescriptionScrollDirection::Up);
            },
            true, true);
        registerAction(
            "", brls::BUTTON_NAV_DOWN,
            [this](brls::View*) {
                return Scroll(DescriptionScrollDirection::Down);
            },
            true, true);
        registerAction(
            "", brls::BUTTON_B,
            [this](brls::View*) {
                ReturnToButtons();
                return true;
            },
            true);
    }

    void onFocusGained() override
    {
        brls::ScrollingFrame::onFocusGained();
        setBorderThickness(3.0f);
        if (focus_hint_)
            focus_hint_->setVisibility(brls::Visibility::VISIBLE);
    }

    void onFocusLost() override
    {
        brls::ScrollingFrame::onFocusLost();
        setBorderThickness(0.0f);
        if (focus_hint_)
            focus_hint_->setVisibility(brls::Visibility::GONE);
    }

  private:
    bool Scroll(DescriptionScrollDirection direction)
    {
        const float maximum_offset =
            std::max(0.0f, getContentHeight() - getHeight());
        const DescriptionScrollResult result = AdvanceDescriptionScroll(
            getContentOffsetY(), maximum_offset, direction, 48.0f);
        setContentOffsetY(result.offset, false);
        if (result.return_to_buttons)
            ReturnToButtons();
        return true;
    }

    void ReturnToButtons()
    {
        if (return_focus_)
            brls::Application::giveFocus(return_focus_);
    }

    brls::View* return_focus_ = nullptr;
    brls::Label* focus_hint_ = nullptr;
};

brls::Label* MakeLabel(const std::string& text, float size, NVGcolor color, float bottom_margin = 10.0f)
{
    auto* label = new brls::Label();
    label->setText(Tr(text));
    label->setFontSize(size);
    label->setTextColor(color);
    label->setMarginBottom(bottom_margin);
    return label;
}

float MeasureWrappedLabelHeight(
    brls::Label* label, const std::string& text, float width)
{
    if (!label || text.empty() || width <= 0.0f)
        return 0.0f;

    NVGcontext* vg = brls::Application::getNVGContext();
    nvgSave(vg);
    nvgFontSize(vg, label->getFontSize());
    nvgFontFaceId(vg, label->getFont());
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgTextLineHeight(vg, label->getLineHeight());
    float bounds[4] {};
    nvgTextBoxBounds(vg, 0.0f, 0.0f, width, text.c_str(), nullptr, bounds);
    nvgRestore(vg);
    return std::max(0.0f, bounds[3] - bounds[1]);
}

std::string PrimaryStore(const GameInfo& game)
{
    if (!game.available_stores.empty())
        return game.available_stores.front();

    if (!game.publisher.empty())
        return game.publisher;

    return "GeForce NOW";
}

std::string JoinStores(const GameInfo& game)
{
    if (game.available_stores.empty())
        return PrimaryStore(game);

    std::ostringstream stream;
    for (size_t i = 0; i < game.available_stores.size(); ++i)
    {
        if (i > 0)
            stream << ", ";
        stream << game.available_stores[i];
    }
    return stream.str();
}

std::string SafeText(const std::string& value, const std::string& fallback)
{
    return value.empty() ? fallback : value;
}

std::string VariantLabel(const GameVariant& variant)
{
    std::string label = SafeText(variant.store, Tr("Unknown store"));
    if (variant.library_selected)
        label += " (" + Tr("selected in library") + ")";
    else if (!variant.library_status.empty())
        label += " (" + variant.library_status + ")";
    return label;
}

std::string HumanizeMetadata(std::string value)
{
    bool capitalize = true;
    for (char& character : value)
    {
        if (character == '_' || character == '-')
        {
            character = ' ';
            capitalize = true;
            continue;
        }
        if (capitalize)
        {
            character = static_cast<char>(
                std::toupper(static_cast<unsigned char>(character)));
            capitalize = false;
        }
        else
        {
            character = static_cast<char>(
                std::tolower(static_cast<unsigned char>(character)));
        }
    }
    return value;
}

std::string JoinMetadata(
    const std::vector<std::string>& values, size_t maximum = 4)
{
    std::ostringstream stream;
    const size_t count = std::min(maximum, values.size());
    for (size_t index = 0; index < count; ++index)
    {
        if (index > 0)
            stream << "  /  ";
        stream << Tr(HumanizeMetadata(values[index]));
    }
    return stream.str();
}

brls::Box* MakeTagRow(const std::vector<std::string>& values)
{
    auto* row = new brls::Box(brls::Axis::ROW);
    row->setHeight(34);
    row->setMarginBottom(8);
    const size_t count = std::min<size_t>(4, values.size());
    for (size_t index = 0; index < count; ++index)
    {
        const std::string text = Tr(HumanizeMetadata(values[index]));
        auto* tag = new brls::Box(brls::Axis::ROW);
        tag->setHeight(28);
        tag->setWidth(std::min(142.0f, 24.0f + text.size() * 7.7f));
        tag->setMarginRight(index + 1 < count ? 7.0f : 0.0f);
        tag->setPadding(4, 9, 4, 9);
        tag->setCornerRadius(6);
        tag->setAlignItems(brls::AlignItems::CENTER);
        tag->setJustifyContent(brls::JustifyContent::CENTER);
        tag->setBackgroundColor(nvgRGB(31, 35, 41));
        tag->addView(MakeLabel(text, 13.0f, nvgRGB(195, 202, 211), 0.0f));
        row->addView(tag);
    }
    return row;
}

bool IsNumericLaunchId(const std::string& id)
{
    return !id.empty() && std::all_of(id.begin(), id.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
}

std::string PromptCredential(
    const std::string& title, const std::string& initial, bool password)
{
#ifdef __SWITCH__
    SwkbdConfig keyboard {};
    if (R_FAILED(swkbdCreate(&keyboard, 0)))
        return {};
    if (password)
        swkbdConfigMakePresetPassword(&keyboard);
    else
        swkbdConfigMakePresetDefault(&keyboard);
    swkbdConfigSetType(&keyboard, SwkbdType_Latin);
    swkbdConfigSetStringLenMax(&keyboard, password ? 128 : 254);
    swkbdConfigSetHeaderText(&keyboard, title.c_str());
    const std::string save = Tr("Save");
    swkbdConfigSetOkButtonText(&keyboard, save.c_str());
    if (!initial.empty() && !password)
        swkbdConfigSetInitialText(&keyboard, initial.c_str());
    std::array<char, 512> output {};
    const Result result = swkbdShow(&keyboard, output.data(), output.size());
    swkbdClose(&keyboard);
    if (R_FAILED(result))
        return {};
    return output.data();
#else
    (void)title;
    (void)initial;
    (void)password;
    return {};
#endif
}

} // namespace

GameDetailView::GameDetailView(const GfnClient& client, GameDetailData data)
    : brls::Box(brls::Axis::COLUMN)
    , client_(client)
    , data_(std::move(data))
{
    const std::string saved_variant = client_.LoadLauncherPreference(ActiveUserId(), data_.game_id);
    for (size_t i = 0; i < data_.variants.size(); ++i)
    {
        if (!saved_variant.empty() && data_.variants[i].id == saved_variant)
        {
            selected_variant_index_ = i;
            launcher_preference_loaded_ = true;
            break;
        }
        if (!launcher_preference_loaded_ && data_.variants[i].id == data_.launch_app_id)
            selected_variant_index_ = i;
    }

    setPadding(16, 30, 18, 30);
    setBackgroundColor(nvgRGB(11, 12, 15));

    auto* title_row = new brls::Box(brls::Axis::ROW);
    title_row->setHeight(68);
    title_row->setAlignItems(brls::AlignItems::CENTER);
    title_row->setMarginBottom(6);
    auto* title = MakeLabel(data_.title, 39.0f, nvgRGB(248, 249, 251), 0.0f);
    title->setGrow(1.0f);
    title_row->addView(title);
    title_row->addView(MakeLabel(
        data_.subtitle, 15.0f, nvgRGB(137, 145, 156), 0.0f));
    addView(title_row);

    auto* content = new brls::Box(brls::Axis::ROW);
    content->setGrow(1.0f);
    content->setMarginTop(8);

    auto* info_column = new brls::Box(brls::Axis::COLUMN);
    info_column->setWidth(405);
    info_column->setMarginRight(24);
    info_column->setAlignItems(brls::AlignItems::STRETCH);

    auto* identity_text = new brls::Box(brls::Axis::COLUMN);
    identity_text->setMarginBottom(12);
    identity_text->addView(MakeLabel(
        Tr(data_.owned ? "In your library" : "Supported in GeForce NOW"),
        19.0f, nvgRGB(77, 218, 130), 8.0f));
    if (!data_.content_ratings.empty())
    {
        identity_text->addView(MakeLabel(
            JoinMetadata(data_.content_ratings, 2),
            14.0f, nvgRGB(219, 223, 229), 7.0f));
    }
    if (!data_.genres.empty())
        identity_text->addView(MakeTagRow(data_.genres));
    if (!data_.supported_controls.empty())
    {
        identity_text->addView(MakeLabel(
            Tr("Input") + ": " + JoinMetadata(data_.supported_controls, 3),
            13.5f, nvgRGB(151, 159, 170), 6.0f));
    }
    if (!data_.membership_tier_label.empty())
    {
        identity_text->addView(MakeLabel(
            Tr("Membership") + ": " + data_.membership_tier_label,
            13.5f, nvgRGB(246, 196, 80), 0.0f));
    }
    info_column->addView(identity_text);

    const bool has_nte_actions = IsNevernessToEverness(data_.title);
    auto* actions = new brls::Box(brls::Axis::COLUMN);
    actions->setMarginBottom(10);

    auto* play_button = new brls::Button();
    play_button->setStyle(&brls::BUTTONSTYLE_PRIMARY);
    play_button->setHeight(50);
    play_button->setCornerRadius(9);
    play_button->setText(Tr(data_.owned ? "Play on GeForce NOW" : "Play from Store"));
    play_button->registerClickAction([this](brls::View* view) {
        (void)view;
        Play();
        return true;
    });
    actions->addView(play_button);

    auto* secondary_actions = new brls::Box(brls::Axis::ROW);
    secondary_actions->setHeight(44);
    secondary_actions->setMarginTop(8);

    store_button_ = new brls::Button();
    store_button_->setHeight(44);
    store_button_->setCornerRadius(8);
    store_button_->setGrow(1.0f);
    store_button_->registerClickAction([this](brls::View*) {
        ShowStoreSelector(false);
        return true;
    });
    UpdateStoreButton();
    secondary_actions->addView(store_button_);

    auto* shortcut_button = new brls::Button();
    shortcut_button->setStyle(&brls::BUTTONSTYLE_BORDERED);
    shortcut_button->setWidth(52);
    shortcut_button->setHeight(44);
    shortcut_button->setCornerRadius(8);
    shortcut_button->setMarginLeft(8);
    shortcut_button->setText("+");
    shortcut_button->registerClickAction([this](brls::View*) {
        CreateSwitchShortcut();
        return true;
    });
    secondary_actions->addView(shortcut_button);
    actions->addView(secondary_actions);

    const std::string studio = !data_.developer.empty()
        ? data_.developer
        : data_.publisher;
    actions->addView(MakeLabel(
        Tr("By") + " " + SafeText(studio, Tr("Unknown studio")) +
            "  /  " + SafeText(data_.stores, Tr("Unknown store")),
        14.0f, nvgRGB(166, 174, 184), 7.0f));

    if (has_nte_actions)
    {
        nte_button_ = new brls::Button();
        nte_button_->setHeight(44);
        nte_button_->setMarginBottom(8);
        nte_button_->registerClickAction([this](brls::View*) {
            OpenNteCredentialsMenu();
            return true;
        });
        UpdateNteButton();
        actions->addView(nte_button_);
    }
    info_column->addView(actions);

    auto* description_header = new brls::Box(brls::Axis::ROW);
    description_header->setHeight(31);
    description_header->setAlignItems(brls::AlignItems::CENTER);
    auto* description_title = MakeLabel(
        "About this game", 16.0f, nvgRGB(246, 248, 250), 0.0f);
    description_title->setGrow(1.0f);
    description_header->addView(description_title);
    auto* description_focus_hint = MakeLabel(
        "Up / Down  Scroll  |  B  Back",
        12.0f,
        nvgRGB(105, 220, 148),
        0.0f);
    description_focus_hint->setVisibility(brls::Visibility::GONE);
    description_header->addView(description_focus_hint);
    info_column->addView(description_header);

    const std::string description_text = Tr(SafeText(
        data_.description,
        "No description is available yet for this title."));
    auto* description = MakeLabel(
        description_text,
        15.5f,
        nvgRGB(232, 235, 240),
        0.0f);
    description->setSingleLine(false);
    description->setWidth(365);
    // Label defaults to max-height: 100%, which truncates wrapped text to the
    // viewport and makes ScrollingFrame think there is almost nothing to
    // scroll. Keep the full measured text height in the detached content.
    description->setMaxHeight(brls::View::AUTO);

    auto* description_content = new brls::Box(brls::Axis::COLUMN);
    description_content->setWidth(389);
    description_content->setPadding(10, 12, 10, 12);
    description_content->addView(description);

    brls::View* description_return_focus =
        nte_button_ ? static_cast<brls::View*>(nte_button_)
                    : static_cast<brls::View*>(store_button_);
    auto* description_frame = new DescriptionScrollingFrame(
        description_return_focus, description_focus_hint);
    const float measured_height =
        MeasureWrappedLabelHeight(description, description_text, 365.0f);
    const float maximum_height = has_nte_actions ? 205.0f : 255.0f;
    const float viewport_height = std::clamp(
        std::ceil(measured_height + 20.0f), 46.0f, maximum_height);
    description_frame->setHeight(viewport_height);
    description_frame->setShrink(1.0f);
    description_frame->setCornerRadius(9);
    description_frame->setBackgroundColor(nvgRGB(20, 23, 28));
    description_frame->setContentView(description_content);
    info_column->addView(description_frame);

    content->addView(info_column);

    auto* media_column = new brls::Box(brls::Axis::COLUMN);
    media_column->setGrow(1.0f);
    media_column->setAlignItems(brls::AlignItems::STRETCH);

    media_urls_ = data_.screenshots;
    if (media_urls_.empty() && !data_.hero_url.empty())
        media_urls_.push_back(data_.hero_url);
    if (media_urls_.empty() && !data_.image_url.empty())
        media_urls_.push_back(data_.image_url);

    screenshot_image_ = new brls::Image();
    screenshot_image_->setHeight(420);
    screenshot_image_->setCornerRadius(13);
    screenshot_image_->setScalingType(brls::ImageScalingType::FILL);
    screenshot_image_->setMarginBottom(8);
    media_column->addView(screenshot_image_);

    auto* dots = new brls::Box(brls::Axis::ROW);
    dots->setHeight(24);
    dots->setAlignItems(brls::AlignItems::CENTER);
    dots->setJustifyContent(brls::JustifyContent::CENTER);
    dots->setMarginBottom(6);
    for (size_t index = 0; index < media_urls_.size(); ++index)
    {
        auto* dot = MakeLabel("●", 14.0f, nvgRGB(94, 101, 110), 0.0f);
        dot->setWidth(20);
        dot->setHorizontalAlign(brls::HorizontalAlign::CENTER);
        screenshot_dots_.push_back(dot);
        dots->addView(dot);
    }
    media_column->addView(dots);

    if (media_urls_.size() > 1)
    {
        media_column->addView(MakeLabel(
            "L / R  " + Tr("Browse screenshots"),
            13.0f, nvgRGB(132, 140, 151), 5.0f));
    }
    if (!data_.feature_labels.empty())
        media_column->addView(MakeTagRow(data_.feature_labels));
    media_column->addView(MakeLabel(
        Tr("Publisher") + ": " + SafeText(data_.publisher, Tr("Unknown")) +
            "  /  " + Tr("Last played") + ": " +
            SafeText(data_.last_played, Tr("Never")),
        13.5f, nvgRGB(137, 145, 156), 0.0f));

    UpdateScreenshot();
    content->addView(media_column);
    addView(content);

    if (media_urls_.size() > 1)
    {
        registerAction(Tr("Previous screenshot"), brls::BUTTON_LB, [this](brls::View*) {
            ShowPreviousScreenshot();
            return true;
        });
        registerAction(Tr("Next screenshot"), brls::BUTTON_RB, [this](brls::View*) {
            ShowNextScreenshot();
            return true;
        });
    }

    registerAction(Tr("Back"), brls::BUTTON_B, [](brls::View* view) {
        (void)view;
        brls::Application::popActivity();
        return true;
    });
}

GameDetailView::~GameDetailView()
{
    alive_->store(false);
    screenshot_generation_->fetch_add(1);
}

void GameDetailView::ShowPreviousScreenshot()
{
    if (media_urls_.empty())
        return;
    screenshot_index_ =
        (screenshot_index_ + media_urls_.size() - 1) % media_urls_.size();
    UpdateScreenshot();
}

void GameDetailView::ShowNextScreenshot()
{
    if (media_urls_.empty())
        return;
    screenshot_index_ = (screenshot_index_ + 1) % media_urls_.size();
    UpdateScreenshot();
}

void GameDetailView::UpdateScreenshot()
{
    if (!screenshot_image_ || media_urls_.empty())
        return;
    screenshot_index_ = std::min(screenshot_index_, media_urls_.size() - 1);
    const std::uint64_t generation =
        screenshot_generation_->fetch_add(1) + 1;
    const auto generation_state = screenshot_generation_;
    SetCachedGalleryImage(
        screenshot_image_,
        media_urls_[screenshot_index_],
        [generation_state, generation]() {
            return generation_state->load() == generation;
        });

    if (media_urls_.size() > 1)
    {
        const size_t previous =
            (screenshot_index_ + media_urls_.size() - 1) % media_urls_.size();
        const size_t next = (screenshot_index_ + 1) % media_urls_.size();
        PrefetchCachedGalleryImage(media_urls_[previous]);
        if (next != previous)
            PrefetchCachedGalleryImage(media_urls_[next]);
    }

    for (size_t index = 0; index < screenshot_dots_.size(); ++index)
    {
        screenshot_dots_[index]->setTextColor(
            index == screenshot_index_
                ? nvgRGB(105, 232, 151)
                : nvgRGB(91, 98, 108));
    }
}

void GameDetailView::Play()
{
    const auto& session = AppState::Instance().session();
    if (!session)
    {
        ShowError("Not Logged In", "Sign in from Library before starting a GeForce NOW session.");
        return;
    }

    if (data_.variants.size() > 1 && !launcher_preference_loaded_)
    {
        ShowStoreSelector(true);
        return;
    }

    LaunchSelectedVariant();
}

std::string GameDetailView::ActiveUserId() const
{
    const auto& session = AppState::Instance().session();
    return session ? session->user.user_id : "";
}

void GameDetailView::UpdateStoreButton()
{
    if (!store_button_)
        return;
    const std::string store = selected_variant_index_ < data_.variants.size()
        ? SafeText(data_.variants[selected_variant_index_].store, "Unknown")
        : SafeText(data_.stores, "Unknown");
    store_button_->setText(Tr("Store") + ": " + store + (data_.variants.size() > 1 ? " (" + Tr("change") + ")" : ""));
}

void GameDetailView::UpdateNteButton()
{
    if (!nte_button_)
        return;
    nte_button_->setText(
        LoadNteCredentials().valid()
            ? Tr("NTE Auto-login: Ready") + " (L + X)"
            : Tr("NTE Auto-login: Set email and password"));
}

void GameDetailView::ConfigureNteCredentials()
{
    NteCredentials credentials = LoadNteCredentials();
    const std::string email = PromptCredential(
        "NTE email address", credentials.email, false);
    if (email.empty())
    {
        brls::Application::notify("NTE credential setup cancelled");
        return;
    }

    const std::string password = PromptCredential("NTE password", {}, true);
    if (password.empty())
    {
        brls::Application::notify("NTE credential setup cancelled");
        return;
    }

    credentials.email = email;
    credentials.password = password;
    if (!SaveNteCredentials(credentials))
    {
        ShowError("NTE Auto-login", "Could not write " + NteCredentialsPath());
        return;
    }
    std::fill(credentials.password.begin(), credentials.password.end(), '\0');
    UpdateNteButton();
    brls::Application::notify("NTE Auto-login saved; press L + X during the NTE sign-in screen");
}

void GameDetailView::OpenNteCredentialsMenu()
{
    const bool configured = LoadNteCredentials().valid();
    if (!configured)
    {
        ConfigureNteCredentials();
        return;
    }

    auto* dialog = new brls::Dialog(
        "NTE Auto-login\n\nCredentials are stored as plain text at:\n" +
        NteCredentialsPath() +
        "\n\nPress L + X on the first NTE email sign-in screen. B cancels an active sequence.");
    dialog->addButton("Edit credentials", [this] { ConfigureNteCredentials(); });
    dialog->addButton("Clear credentials", [this] {
        ClearNteCredentials();
        UpdateNteButton();
        brls::Application::notify("NTE Auto-login credentials removed");
    });
    dialog->addButton("Cancel", [] {});
    dialog->setCancelable(true);
    dialog->open();
}

void GameDetailView::ShowStoreSelector(bool launch_after_selection)
{
    if (data_.variants.empty())
    {
        LaunchSelectedVariant();
        return;
    }

    std::vector<std::string> labels;
    labels.reserve(data_.variants.size());
    for (const auto& variant : data_.variants)
        labels.push_back(VariantLabel(variant));

    auto* dropdown = new brls::Dropdown(
        Tr("Choose game store"), labels,
        [this](int selected) {
            if (selected < 0 || static_cast<size_t>(selected) >= data_.variants.size())
                return;
            selected_variant_index_ = static_cast<size_t>(selected);
            launcher_preference_loaded_ = true;
            const auto& variant = data_.variants[selected_variant_index_];
            data_.launch_app_id = variant.id;
            client_.SaveLauncherPreference(ActiveUserId(), data_.game_id, variant.id);
            UpdateStoreButton();
            brls::Application::notify("Store selected: " + SafeText(variant.store, variant.id));
        }, static_cast<int>(selected_variant_index_),
        [this, launch_after_selection](int selected) {
            if (launch_after_selection && selected >= 0)
                LaunchSelectedVariant();
        });
    brls::Application::pushActivity(new brls::Activity(dropdown));
}

void GameDetailView::LaunchSelectedVariant()
{
    std::string launch_app_id = data_.launch_app_id;
    std::string store = data_.stores;
    if (selected_variant_index_ < data_.variants.size())
    {
        launch_app_id = data_.variants[selected_variant_index_].id;
        store = data_.variants[selected_variant_index_].store;
    }

    if (launch_app_id.empty())
    {
        ShowError("Launch Error", "This game has no launch App ID.");
        return;
    }

    LaunchSessionDialog(client_, *AppState::Instance().session(), launch_app_id,
                        data_.title, store, data_.title, data_.game_id,
                        data_.image_url);
}

void GameDetailView::CreateSwitchShortcut()
{
    shortcut::LaunchRequest request;
    request.launch_app_id = data_.launch_app_id;
    request.store = data_.stores;
    if (selected_variant_index_ < data_.variants.size())
    {
        request.launch_app_id = data_.variants[selected_variant_index_].id;
        request.store = data_.variants[selected_variant_index_].store;
    }
    request.game_id = data_.game_id;
    request.title = data_.title;
    request.image_url = data_.image_url;
    if (!shortcut::IsValid(request))
    {
        ShowError(
            Tr("Shortcut unavailable"),
            Tr("This game does not expose a valid GeForce NOW launch ID."));
        return;
    }

    auto* confirmation = new brls::Dialog(
        Tr("Create a personalized shortcut for this game?\n\n"
           "The shortcut uses the selected store and game artwork. It can "
           "start the game directly through your saved NVStreamNX account.\n\n"
           "Installing it on the Horizon HOME screen is offered separately."));
    confirmation->addButton(
        Tr("Create shortcut"),
        [request = std::move(request), alive = alive_]() mutable {
            brls::Application::notify(Tr("Creating game shortcut..."));
            brls::async([request = std::move(request), alive]() mutable {
                shortcut::CreateResult result =
                    shortcut::CreateGameShortcut(std::move(request));
                brls::sync([result = std::move(result), alive]() {
                    if (!alive->load())
                        return;
                    if (!result.success)
                    {
                        ShowError(Tr("Shortcut creation failed"), result.error);
                        return;
                    }

                    std::string body = Tr(
                        "The game shortcut is ready in the NVStreamNX Shortcuts "
                        "folder.\n\nInstall it to SD storage now so it appears "
                        "directly on the Horizon HOME screen?");
                    if (!result.used_game_cover)
                    {
                        body += "\n\n" + Tr(
                            "NVStreamNX artwork was used because the game cover "
                            "could not be converted safely.");
                    }

                    auto* install = new brls::Dialog(body);
                    install->addButton(
                        Tr("Install on HOME"),
                        [path = result.nro_path, title = result.title] {
                            std::string error;
                            if (!shortcut::StartForwarderInstaller(
                                    path, title, error))
                            {
                                ShowError(Tr("HOME installation failed"), error);
                            }
                        });
                    install->addButton(Tr("Keep for hbmenu"), [] {});
                    install->setCancelable(true);
                    install->open();
                });
            }, false);
        });
    confirmation->addButton(Tr("Cancel"), [] {});
    confirmation->setCancelable(true);
    confirmation->open();
}

GameDetailData MakeLibraryGameDetail(const GameInfo& game)
{
    GameDetailData data;
    data.title         = game.title;
    data.game_id       = game.uuid.empty() ? game.id : game.uuid;
    data.subtitle      = PrimaryStore(game) + " " + Tr("library title");
    data.image_url     = game.image_url;
    data.poster_url    = game.poster_url;
    data.hero_url      = game.hero_url;
    data.screenshots   = game.screenshots;
    data.launch_app_id = game.launch_app_id;
    data.publisher     = game.publisher;
    data.developer     = game.developer;
    data.description   = game.description;
    data.stores        = JoinStores(game);
    data.membership_tier_label = game.membership_tier_label;
    data.last_played   = game.last_played;
    data.genres        = game.genres;
    data.feature_labels = game.feature_labels;
    data.content_ratings = game.content_ratings;
    data.supported_controls = game.supported_controls;
    data.owned         = true;
    data.variants      = game.variants;
    data.variants.erase(
        std::remove_if(data.variants.begin(), data.variants.end(), [](const GameVariant& variant) {
            return !IsNumericLaunchId(variant.id);
        }),
        data.variants.end());
    return data;
}

GameDetailData MakeCatalogGameDetail(const PublicGame& game)
{
    GameDetailData data;
    data.title         = game.title;
    data.game_id       = game.uuid.empty() ? game.id : game.uuid;
    data.subtitle      = game.store + " " +
        Tr(game.is_in_library ? "library title" : "catalog title");
    data.image_url     = game.image_url;
    data.poster_url    = game.poster_url;
    data.hero_url      = game.hero_url;
    data.screenshots   = game.screenshots;
    data.launch_app_id = game.launch_app_id.empty() ? game.id : game.launch_app_id;
    data.publisher     = game.publisher;
    data.developer     = game.developer;
    data.description   = game.description;
    data.stores        = game.store;
    data.membership_tier_label = game.membership_tier_label;
    data.genres        = game.genres;
    data.feature_labels = game.feature_labels;
    data.content_ratings = game.content_ratings;
    data.supported_controls = game.supported_controls;
    data.owned         = game.is_in_library;
    data.variants      = game.variants;
    if (data.variants.empty())
    {
        GameVariant variant;
        variant.id = game.id;
        variant.store = game.store;
        data.variants.push_back(std::move(variant));
    }
    return data;
}

} // namespace opennow
