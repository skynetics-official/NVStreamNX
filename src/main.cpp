#include <switch.h>
#include <stdio.h>
#include "config.hpp"
#include "sysclk_manager.hpp"

int main(int argc, char **argv) {
    consoleInit(NULL);
    
    // Инициализация геймпада (Joy-Con, Pro Controller или портативный режим)
    padConfigureInput(1, HidNpadStyle_FullKey | HidNpadStyle_Handheld);
    PadState pad;
    padInitializeDefault(&pad);

    printf("\x1b[1;1H NVStreamNX v1.0 - NVIDIA Cloud Manager");
    printf("\x1b[3;1H----------------------------------------");
    printf("\x1b[5;1HBitrate: %d Mbps", Config::getInstance().getBitrate());
    printf("\x1b[6;1HBuffer Delay: %d ms", Config::getInstance().getBufferDelayMs());
    printf("\x1b[7;1HOC Profile: %d", Config::getInstance().getOCProfile());
    
    // Применение профиля разгона
    SysClkManager::applyProfile(Config::getInstance().getOCProfile());

    printf("\x1b[10;1HPress [PLUS] to exit.");

    // Основной цикл программы
    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break;

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
