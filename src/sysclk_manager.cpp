#include "sysclk_manager.hpp"

// libnx через appletSetCpuBoostMode поддерживает только два режима: 0 и 1.
// Поэтому профили Boost и MAX OC здесь отличить нельзя — оба включают FastLoad.
// Для реальных 1224/1785 MHz нужен sys-clk (см. README).
void SysClkManager::applyProfile(int profileIndex) {
    if (profileIndex == 0) {
        appletSetCpuBoostMode(ApmCpuBoostMode_Normal);
    } else if (profileIndex == 1 || profileIndex == 2) {
        appletSetCpuBoostMode(ApmCpuBoostMode_FastLoad);
    }
}
