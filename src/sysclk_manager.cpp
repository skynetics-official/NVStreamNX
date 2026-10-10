#include "sysclk_manager.hpp"

void SysClkManager::applyProfile(int profileIndex) {
    if (profileIndex == 0) {
        appletSetCpuBoostMode(AppletCpuBoostMode_Disabled);
    } 
    else if (profileIndex == 1 || profileIndex == 2) {
        appletSetCpuBoostMode(AppletCpuBoostMode_FastLoad);
    }
}
