#include <borealis.hpp>
#include "config.hpp"
#include "sysclk_manager.hpp"

// Вкладка основных настроек потока
class StreamSettingsTab : new brls::List {
public:
    StreamSettingsTab() {
        // 1. Настройка Битрейта
        auto* bitrateSlider = new brls::SliderCell();
        bitrateSlider->setTitle("Максимальный битрейт");
        bitrateSlider->init(5, 50, Config::getInstance().getBitrate());
        bitrateSlider->getEvent()->subscribe([](float value) {
            Config::getInstance().setBitrate(static_cast<int>(value));
        });
        this->addView(bitrateSlider);

        // 2. Разрешение потока
        auto* resSelector = new brls::SelectorCell();
        resSelector->setTitle("Разрешение картинки");
        resSelector->init({"720p (Портативный режим)", "1080p (ТВ / Dock)"}, Config::getInstance().getResolutionIndex());
        resSelector->getEvent()->subscribe([](int index) {
            Config::getInstance().setResolutionIndex(index);
        });
        this->addView(resSelector);

        // 3. Задержка сетевого буфера
        auto* bufferSelector = new brls::SelectorCell();
        bufferSelector->setTitle("Сетевой буфер (Latency)");
        bufferSelector->init({"Ультра-низкий (0ms)", "Баланс (10ms)", "Стабильный (30ms)"}, Config::getInstance().getBufferIndex());
        bufferSelector->getEvent()->subscribe([](int index) {
            Config::getInstance().setBufferIndex(index);
        });
        this->addView(bufferSelector);
    }
};

// Вкладка настроек производительности и системы
class SystemSettingsTab : public brls::List {
public:
    SystemSettingsTab() {
        // 1. Профиль разгона
        auto* ocSelector = new brls::SelectorCell();
        ocSelector->setTitle("Профиль производительности (Boost)");
        ocSelector->init({"Выключен (Stock)", "Boost (1224 MHz CPU)", "MAX OC (1785 MHz CPU)"}, Config::getInstance().getOCProfile());
        ocSelector->getEvent()->subscribe([](int index) {
            Config::getInstance().setOCProfile(index);
            SysClkManager::applyProfile(index);
        });
        this->addView(ocSelector);

        // 2. Оверлей статистики
        auto* statsToggle = new brls::ToggleCell();
        statsToggle->setTitle("Показывать FPS и Ping поверх игры");
        statsToggle->init(Config::getInstance().getStatsOverlay());
        statsToggle->getEvent()->subscribe([](bool value) {
            Config::getInstance().setStatsOverlay(value);
        });
        this->addView(statsToggle);
    }
};

int main(int argc, char* argv[]) {
    // Инициализация библиотеки Borealis UI
    if (!brls::Application::init("NVStreamNX - Cloud Manager")) {
        return 0;
    }

    // Применяем стартовый разгон
    SysClkManager::applyProfile(Config::getInstance().getOCProfile());

    // Создаем главное окно
    brls::AppletFrame* frame = new brls::AppletFrame();
    frame->setHeaderTitle("NVStreamNX");
    frame->setIcon("romfs:/icon.png");

    // Добавляем вкладки настроек
    frame->addTab("Поток и Сеть", new StreamSettingsTab());
    frame->addTab("Разгон и Система", new SystemSettingsTab());

    brls::Application::pushView(frame);

    // Главный цикл отрисовки интерфейса
    while (brls::Application::mainLoop()) {
    }

    return 0;
}
