#include <borealis.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include "config.hpp"
#include "sysclk_manager.hpp"

// Вкладка основных настроек потока
class StreamSettingsTab : public brls::Box {
public:
    StreamSettingsTab() : brls::Box(brls::Axis::COLUMN) {
        this->setPadding(32, 40, 47, 80);
        // 1. Настройка Битрейта
        auto* bitrateSlider = new brls::SliderCell();
        const float kMinMbps = 5.0f, kMaxMbps = 50.0f;
        bitrateSlider->init(
            "Максимальный битрейт",
            (Config::getInstance().getBitrate() - kMinMbps) / (kMaxMbps - kMinMbps),
            [bitrateSlider, kMinMbps, kMaxMbps](float progress) {
                int mbps = static_cast<int>(kMinMbps + progress * (kMaxMbps - kMinMbps) + 0.5f);
                Config::getInstance().setBitrate(mbps);
                bitrateSlider->setDetailText(std::to_string(mbps));
            });
        bitrateSlider->setDetailText(std::to_string(Config::getInstance().getBitrate()));
        this->addView(bitrateSlider);

        // 2. Разрешение потока
        auto* resSelector = new brls::SelectorCell();
        resSelector->init("Разрешение картинки", std::vector<std::string>{"720p (Портативный режим)", "1080p (ТВ / Dock)"}, Config::getInstance().getResolutionIndex(), [](int index) {
            Config::getInstance().setResolutionIndex(index);
        });
        this->addView(resSelector);

        // 3. Задержка сетевого буфера
        auto* bufferSelector = new brls::SelectorCell();
        bufferSelector->init("Сетевой буфер (Latency)", std::vector<std::string>{"Ультра-низкий (0ms)", "Баланс (10ms)", "Стабильный (30ms)"}, Config::getInstance().getBufferIndex(), [](int index) {
            Config::getInstance().setBufferIndex(index);
        });
        this->addView(bufferSelector);
    }
};

// Вкладка настроек производительности и системы
class SystemSettingsTab : public brls::Box {
public:
    SystemSettingsTab() : brls::Box(brls::Axis::COLUMN) {
        this->setPadding(32, 40, 47, 80);
        // 1. Профиль разгона
        auto* ocSelector = new brls::SelectorCell();
        ocSelector->init("Профиль производительности (Boost)", std::vector<std::string>{"Выключен (Stock)", "Boost (1224 MHz CPU)", "MAX OC (1785 MHz CPU)"}, Config::getInstance().getOCProfile(), [](int index) {
            Config::getInstance().setOCProfile(index);
            SysClkManager::applyProfile(index);
        });
        this->addView(ocSelector);

        // 2. Оверлей статистики
        auto* statsToggle = new brls::BooleanCell();
        statsToggle->init("Показывать FPS и Ping поверх игры", Config::getInstance().getStatsOverlay(), [](bool value) {
            Config::getInstance().setStatsOverlay(value);
        });
        this->addView(statsToggle);
    }
};

int main(int argc, char* argv[]) {
    // Инициализация библиотеки Borealis UI
    if (!brls::Application::init()) {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }
    brls::Application::createWindow("NVStreamNX - Cloud Manager");

    // Применяем стартовый профиль разгона
    SysClkManager::applyProfile(Config::getInstance().getOCProfile());

    // Создаем главное окно: вкладки слева, содержимое справа
    auto* tabs = new brls::TabFrame();
    tabs->addTab("Поток и Сеть", []() { return new StreamSettingsTab(); });
    tabs->addTab("Разгон и Система", []() { return new SystemSettingsTab(); });

    auto* frame = new brls::AppletFrame(tabs);
    frame->setTitle("NVStreamNX");

    brls::Application::pushActivity(new brls::Activity(frame));

    // Главный цикл отрисовки и интерфейса
    while (brls::Application::mainLoop()) {
    }

    return EXIT_SUCCESS;
}
