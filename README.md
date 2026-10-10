# NVStreamNX

Homebrew-приложение для Nintendo Switch (настройки стрима + профиль CPU boost) на базе Borealis UI.

## Сборка

Нужен devkitPro (devkitA64 + libnx) и CMake.

    git clone --recursive https://github.com/xfangfang/borealis.git library/borealis   # если не submodule
    cmake -B build -DCMAKE_BUILD_TYPE=Release -DPLATFORM_SWITCH=ON -DUSE_GLFW=ON
    make -C build NVStreamNX.nro -j

Готовый файл: `build/NVStreamNX.nro`. Через GitHub Actions всё собирается автоматически
(`.github/workflows/build.yml`), NRO появится в Artifacts.

Своя иконка: положите `resources/icon.jpg` (256x256).

## Известные ограничения

- `appletSetCpuBoostMode` в libnx знает только два режима, поэтому «Boost» и «MAX OC» сейчас делают одно и то же.
  Для реальных 1224/1785 MHz нужна интеграция с sys-clk (IPC).
- Настройки (`Config`) не сохраняются между запусками.
- Оверлей FPS/Ping пока только переключатель — сам оверлей не реализован.
