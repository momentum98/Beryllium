cmake_minimum_required(VERSION 3.20)
project(Beryllium C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

find_package(SDL3 QUIET CONFIG)

if (NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, fetching from GitHub...")
    
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.2.0
        GIT_SHALLOW    TRUE
    )
    
    set(SDL_SHARED TRUE  CACHE BOOL "" FORCE)
    set(SDL_STATIC FALSE CACHE BOOL "" FORCE)
    set(SDL_TEST FALSE CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(SDL3)
endif()

set(SOURCE_FILES 
    src/main.c

    src/game/game.c

    src/platform/window_win32.c

    src/core/memory/memory_win32.c

    src/managers/input/input_manager.c

    src/timer/timer_win32.c

    src/logger/logger.c
)

add_executable(Beryllium ${SOURCE_FILES})

target_include_directories(Beryllium 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/types
        ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL3
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Beryllium PRIVATE SDL3::SDL3)

if (WIN32)
    target_link_libraries(Beryllium 
        PRIVATE
            user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32
    )
endif()