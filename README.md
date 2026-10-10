# NVStreamNX

Нативный клиент GeForce NOW для Nintendo Switch (homebrew, `.nro`): каталог и библиотека игр,
вход по аккаунту, WebRTC-стрим H.264 + Opus, аппаратное декодирование NVDEC, виртуальный геймпад.

Нужен собственный аккаунт GeForce NOW. Проект неофициальный и не связан с NVIDIA или Nintendo.

## Установка

1. Скачайте `NVStreamNX.nro` из GitHub Actions (Artifacts → NVStreamNX-NRO) и распакуйте zip.
2. Положите файл в `SD:/switch/NVStreamNX/NVStreamNX.nro`.
3. Запускайте Homebrew Menu в режиме application: удерживайте **R** при запуске установленной игры.

Данные приложения хранятся в `sdmc:/switch/NVStreamNX/`.

## Сборка

Собирается в GitHub Actions (`.github/workflows/build.yml`). Зависимости из `extern/`
(Borealis, libpeer, FFmpeg) workflow скачивает сам из SwitchNOW на закреплённом коммите.

Локально: положите папку `extern/` из SwitchNOW рядом с `CMakeLists.txt`, затем
`cmake -B build/switch -G Ninja && cmake --build build/switch --target NVStreamNX.nro`.

## Что убрано относительно SwitchNOW

- Установщик ярлыков на HOME (forwarder installer): его `.nro` собирается отдельно и в репозитории его нет.
  Остальной код ярлыков на месте, но кнопка установки покажет ошибку.
- Проверка обновлений указывает на `skynetics-official/NVStreamNX` (релизов там пока нет).

## Основа и благодарности

Код основан на [SwitchNOW](https://github.com/Blade-Punisher/SwitchNOW) (Blade-Punisher),
который опирается на [OpenNOW](https://github.com/OpenCloudGaming/OpenNOW),
[OpenNOW-Switch](https://github.com/OpenCloudGaming/OpenNOW-Switch) и
[Moonlight-Switch](https://github.com/XITRIX/Moonlight-Switch), а также на Borealis, libpeer,
FFmpeg (NVTEGRA), devkitPro/libnx/deko3d. Лицензии этих проектов сохраняются.
