#include "sysclk_manager.hpp"
#include <stdio.h>

void SysClkManager::applyProfile(int profileIndex) {
    if (profileIndex == 0) {
        appletSetCpuBoostMode(AppletCpuBoostMode_Disabled);
        printf("\x1b[8;1H[SYS] CPU Boost: Disabled (Stock)   ");
    } 
    else if (profileIndex == 1 || profileIndex == 2) {
        appletSetCpuBoostMode(AppletCpuBoostMode_FastLoad);
        printf("\x1b[8;1H[SYS] CPU Boost: Enabled (1785 MHz) ");
    }
}
