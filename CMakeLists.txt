cmake_minimum_required(VERSION 3.10)

# Borealis (форк xfangfang) лежит в library/borealis
# (git submodule или клонируется в CI автоматически)
set(BOREALIS_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/library/borealis)
set(BOREALIS_LIBRARY ${BOREALIS_DIR}/library)

if (NOT EXISTS ${BOREALIS_LIBRARY}/CMakeLists.txt)
    message(FATAL_ERROR "Borealis не найден. Выполните:\n  git clone --recursive https://github.com/xfangfang/borealis.git library/borealis")
endif ()

include(${BOREALIS_LIBRARY}/cmake/commonOption.cmake)

option(USE_SHARED_LIB "Whether to use shared libs provided by system" OFF)
cmake_dependent_option(USE_SYSTEM_FMT "" OFF "NOT USE_SHARED_LIB" ON)
cmake_dependent_option(USE_SYSTEM_TINYXML2 "" OFF "NOT USE_SHARED_LIB" ON)
cmake_dependent_option(USE_SYSTEM_TWEENY "" OFF "NOT USE_SHARED_LIB" ON)

include(${BOREALIS_LIBRARY}/cmake/toolchain.cmake)

project(NVStreamNX)
set(PROJECT_AUTHOR    "NVStreamNX Team")
set(VERSION_MAJOR     "1")
set(VERSION_MINOR     "0")
set(VERSION_ALTER     "0")
set(PROJECT_RESOURCES ${BOREALIS_DIR}/resources)

if (USE_LIBROMFS)
    add_libromfs(${PROJECT_NAME} ${PROJECT_RESOURCES})
endif ()

add_subdirectory(${BOREALIS_LIBRARY})

find_package(Threads REQUIRED)
list(APPEND APP_PLATFORM_LIB ${CMAKE_THREAD_LIBS_INIT})

file(GLOB_RECURSE MAIN_SRC src/*.cpp)

if (PLATFORM_SWITCH)
    list(APPEND MAIN_SRC ${BOREALIS_LIBRARY}/lib/platforms/switch/switch_wrapper.c)
endif ()

program_target(${PROJECT_NAME} "${MAIN_SRC}")
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

if (PLATFORM_SWITCH)
    # Иконка необязательна: положите resources/icon.jpg (256x256), если нужна своя
    set(ICON_ARG "")
    if (EXISTS ${CMAKE_SOURCE_DIR}/resources/icon.jpg)
        set(ICON_ARG --icon=${CMAKE_SOURCE_DIR}/resources/icon.jpg)
    endif ()

    if (USE_DEKO3D)
        gen_dksh("${PROJECT_RESOURCES}/shaders")
    endif ()

    add_custom_target(${PROJECT_NAME}.nro DEPENDS ${PROJECT_NAME}
        COMMAND ${NX_NACPTOOL_EXE} --create "${PROJECT_NAME}" "${PROJECT_AUTHOR}" "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_ALTER}" ${PROJECT_NAME}.nacp
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_RESOURCES} ${CMAKE_BINARY_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/resources/font
        COMMAND ${NX_ELF2NRO_EXE} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro ${ICON_ARG} --nacp=${PROJECT_NAME}.nacp --romfsdir=${CMAKE_BINARY_DIR}/resources
    )
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE include ${APP_PLATFORM_INCLUDE})
target_compile_options(${PROJECT_NAME} PRIVATE -ffunction-sections -fdata-sections ${APP_PLATFORM_OPTION})
target_link_libraries(${PROJECT_NAME} PRIVATE borealis ${APP_PLATFORM_LIB})
