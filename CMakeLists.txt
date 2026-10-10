cmake_minimum_required(VERSION 3.18)

if (NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "DEVKITPRO is not set. Install devkitPro and export DEVKITPRO before building.")
endif ()

set(DEVKITPRO $ENV{DEVKITPRO})
set(CMAKE_TOOLCHAIN_FILE ${DEVKITPRO}/cmake/Switch.cmake CACHE FILEPATH "devkitPro Switch toolchain")

project(NVStreamNX LANGUAGES C CXX)

set(PLATFORM_SWITCH ON)
# One Deko3D application supports both NVDEC zero-copy and software-decoded
# YUV uploads. Decoder selection is a runtime setting rather than a build type.
set(USE_GLFW OFF CACHE BOOL "Disable GLFW for unified Deko3D" FORCE)
set(USE_DEKO3D ON CACHE BOOL "Use Deko3D on Switch" FORCE)
set(BOREALIS_USE_DEKO3D ON CACHE BOOL "Use Borealis Deko3D driver" FORCE)
set(USE_GLES2 OFF CACHE BOOL "Disable GLES for unified Deko3D" FORCE)
set(USE_SDL2 OFF)
set(USE_GLES3 OFF CACHE BOOL "Disable OpenGL ES 3 on Switch")
set(BRLS_UNITY_BUILD OFF CACHE BOOL "Disable Borealis unity builds")
set(FMT_OS OFF CACHE BOOL "Disable fmt OS-specific APIs for devkitA64")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(JANSSON REQUIRED IMPORTED_TARGET jansson)

add_subdirectory(extern/borealis/library)
add_subdirectory(extern/libpeer)

target_include_directories(borealis PUBLIC ${DEVKITPRO}/portlibs/switch/include)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    app/src/*.cpp
    app/src/*.c
)

set(PLATFORM_SOURCES)

if (PLATFORM_SWITCH)
    list(APPEND PLATFORM_SOURCES
        extern/borealis/library/lib/platforms/switch/switch_wrapper.c
    )
endif ()

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${PLATFORM_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        app/src
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/borealis/library/lib/extern/glfw/deps
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/ffmpeg/include
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/libpeer/src
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/libpeer/third_party/mbedtls/include
)

# Prefer the in-tree libpeer dependencies over portlibs archives with the same
# names. Mixing mbedTLS versions produces TLS failures or an unusable NRO.
target_link_directories(${PROJECT_NAME}
    BEFORE PRIVATE
        ${CMAKE_BINARY_DIR}/dist/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/ffmpeg/lib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_SWITCH __SWITCH__ HAS_SOCKLEN_T=1)
if (PLATFORM_SWITCH)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_DK_RENDERER BOREALIS_USE_DEKO3D OPENNOW_ENABLE_NVDEC)
    find_program(NX_UAM_EXE NAMES uam HINTS "${DEVKITPRO}/tools/bin")
    if (NOT NX_UAM_EXE)
        message(FATAL_ERROR "uam is required for the NVDEC/Deko3D video shaders")
    endif ()
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/resources/shaders")
    set(BOREALIS_DEKO_SHADER_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/borealis/library/lib/extern/nanovg/deko3d/shaders")
    execute_process(COMMAND ${NX_UAM_EXE} -s vert
        -o "${CMAKE_BINARY_DIR}/resources/shaders/fill_vsh.dksh"
        "${BOREALIS_DEKO_SHADER_DIR}/fill_vsh.glsl"
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND ${NX_UAM_EXE} -s frag
        -o "${CMAKE_BINARY_DIR}/resources/shaders/fill_aa_fsh.dksh"
        "${BOREALIS_DEKO_SHADER_DIR}/fill_aa_fsh.glsl"
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND ${NX_UAM_EXE} -s frag
        -o "${CMAKE_BINARY_DIR}/resources/shaders/fill_fsh.dksh"
        "${BOREALIS_DEKO_SHADER_DIR}/fill_fsh.glsl"
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND ${NX_UAM_EXE} -s vert
        -o "${CMAKE_BINARY_DIR}/resources/shaders/opennow_video_vsh.dksh"
        "${CMAKE_CURRENT_SOURCE_DIR}/app/src/stream/deko3d/basic_vsh.glsl"
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND ${NX_UAM_EXE} -s frag
        -o "${CMAKE_BINARY_DIR}/resources/shaders/opennow_video_fsh.dksh"
        "${CMAKE_CURRENT_SOURCE_DIR}/app/src/stream/deko3d/texture_fsh.glsl"
        COMMAND_ERROR_IS_FATAL ANY)
endif ()

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
        -ffunction-sections
        -fdata-sections
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        borealis
        PkgConfig::CURL
        PkgConfig::JANSSON
        peer
        -lavformat
        -lavcodec
        -lavutil
        -lswresample
        z
        bz2
)

set(PROJECT_AUTHOR "NVStreamNX")
set(PROJECT_VERSION_STRING "1.2.0")
set(PROJECT_TITLEID "05004F4E4F575358")
set(PROJECT_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon/icon.jpg)
set(PROJECT_ROMFS_DIR ${CMAKE_BINARY_DIR}/romfs)

add_executable(NVStreamNXShortcut app/shortcut/main.c)
target_compile_options(NVStreamNXShortcut PRIVATE -Wall -Wextra -Werror)

add_custom_target(NVStreamNXShortcut.nro
    DEPENDS NVStreamNXShortcut
    COMMAND ${NX_NACPTOOL_EXE}
        --create
        "NVStreamNX Game Shortcut"
        "NVStreamNX"
        "NVStreamNX"
        NVStreamNXShortcut.nacp
        --titleid=05004F4E53484F52
    COMMAND ${NX_ELF2NRO_EXE}
        NVStreamNXShortcut.elf
        NVStreamNXShortcut.nro
        --icon=${PROJECT_ICON}
        --nacp=NVStreamNXShortcut.nacp
)

add_executable(NVStreamNXUpdater app/updater/main.c)
target_include_directories(NVStreamNXUpdater PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/libpeer/third_party/mbedtls/include
)
target_link_directories(NVStreamNXUpdater BEFORE PRIVATE
    ${CMAKE_BINARY_DIR}/dist/lib
)
target_link_libraries(NVStreamNXUpdater PRIVATE mbedcrypto)
target_compile_options(NVStreamNXUpdater PRIVATE -Wall -Wextra -Werror)

add_custom_target(NVStreamNXUpdater.nro
    DEPENDS NVStreamNXUpdater
    COMMAND ${NX_NACPTOOL_EXE}
        --create
        "NVStreamNX Update Helper"
        "NVStreamNX"
        "${PROJECT_VERSION_STRING}"
        NVStreamNXUpdater.nacp
        --titleid=05004F4E55504454
    COMMAND ${NX_ELF2NRO_EXE}
        NVStreamNXUpdater.elf
        NVStreamNXUpdater.nro
        --icon=${PROJECT_ICON}
        --nacp=NVStreamNXUpdater.nacp
)

add_custom_target(${PROJECT_NAME}.nro
    DEPENDS ${PROJECT_NAME} NVStreamNXShortcut.nro NVStreamNXUpdater.nro
    COMMAND ${NX_NACPTOOL_EXE}
        --create
        "NVStreamNX"
        "${PROJECT_AUTHOR}"
        "${PROJECT_VERSION_STRING}"
        ${PROJECT_NAME}.nacp
        --titleid=${PROJECT_TITLEID}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_ROMFS_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_ROMFS_DIR}/img
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_ROMFS_DIR}/shortcut
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_ROMFS_DIR}/updater
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/font
        ${PROJECT_ROMFS_DIR}/font
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/i18n
        ${PROJECT_ROMFS_DIR}/i18n
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon
        ${PROJECT_ROMFS_DIR}/icon
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/img/sys
        ${PROJECT_ROMFS_DIR}/img/sys
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/material
        ${PROJECT_ROMFS_DIR}/material
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/resources/shaders
        ${PROJECT_ROMFS_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/NVStreamNXShortcut.nro
        ${PROJECT_ROMFS_DIR}/shortcut/NVStreamNXShortcut.nro
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/NVStreamNXUpdater.nro
        ${PROJECT_ROMFS_DIR}/updater/NVStreamNXUpdater.nro
    COMMAND ${NX_ELF2NRO_EXE}
        ${PROJECT_NAME}.elf
        ${PROJECT_NAME}.nro
        --icon=${PROJECT_ICON}
        --nacp=${PROJECT_NAME}.nacp
        --romfsdir=${PROJECT_ROMFS_DIR}
)
