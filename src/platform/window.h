#ifndef WINDOW_H
#define WINDOW_H

#include <SDL_video.h>
#include <windows.h>
#include <types.h>
#include <SDL.h>

typedef struct
{
    bool        isRunning;

    HWND        hWnd;
    SDL_Window* sdlWindow;

    i32         width;
    i32         height;
} Window;

u32 P_SetupWindow(Window* const window, const char* title, const i32 width, const i32 height);
void P_UpdateWindow(Window* const window);
void P_ShutdownWindow(Window* const window);

#endif