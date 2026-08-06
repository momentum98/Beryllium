cmake_minimum_required(VERSION 3.20)
project(Beryllium C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES 
    src/main.c

    src/game/game.c

    src/platform/window.c
)

add_executable(Beryllium ${SOURCE_FILES})

target_include_directories(Beryllium 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/types
        ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL3
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Beryllium 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3/libSDL3.dll.a
        user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32
)