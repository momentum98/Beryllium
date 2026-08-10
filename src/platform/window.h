#ifndef WINDOW_H
#define WINDOW_H

#include <managers/input/input_manager.h>
#include <SDL_video.h>
#include <types.h>
#include <SDL.h>

typedef struct
{
    u8*         buffer;

    u32         size;

    i32         width;
    i32         height;
} ScreenBuffer;

typedef struct
{
    bool         isRunning;

    void*        handle;

    SDL_Window*  sdlWindow;

    i32          width;
    i32          height;

    ScreenBuffer screenBuffer;
    
    void*        backend;
} Window;

u32 P_SetupWindow(Window* const window, const char* title, const i32 width, const i32 height);
void P_SetupScreenBuffer(Window* const window, const i32 width, const i32 height);
u32 P_UpdateWindow(Window* const window, InputManager* const inputManager);
void P_ShutdownWindow(Window* const window);

#endif