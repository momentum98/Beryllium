#include "window.h"

#include <core/memory/memory.h>
#include <SDL_properties.h>
#include <SDL_events.h>
#include <math.h>

#define DEFAULT_SCREEN_BUFFER_WIDTH  320
#define DEFAULT_SCREEN_BUFFER_HEIGHT 200

u32 P_SetupWindow(Window* const window, const char* title, const i32 width, const i32 height)
{
    SDL_Window* const sdlWindow = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);

    if (sdlWindow == NULL)
        return 1;

    const HWND hWnd = SDL_GetPointerProperty(
        SDL_GetWindowProperties(sdlWindow), 
        SDL_PROP_WINDOW_WIN32_HWND_POINTER         , 
        NULL
    );

    if (hWnd == NULL)
    {
        SDL_DestroyWindow(sdlWindow);
        return 1;
    }

    window->isRunning = true;
    window->sdlWindow = sdlWindow;
    window->hWnd = hWnd;

    return 0;
}

void P_UpdateWindow(Window* const window)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            window->isRunning = false;

        if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            if (window->screenBuffer.buffer != NULL)
            {
                M_MemFree(window->screenBuffer.buffer);
                window->screenBuffer.buffer = NULL;
            }

            window->width = event.window.data1;
            window->height = event.window.data2;

            window->screenBuffer.width = min(window->width, DEFAULT_SCREEN_BUFFER_WIDTH);
            window->screenBuffer.height = min(window->height, DEFAULT_SCREEN_BUFFER_HEIGHT);

            window->screenBuffer.size = window->screenBuffer.width * window->screenBuffer.height;

            window->screenBuffer.buffer = M_MemAlloc(window->screenBuffer.size);
        }
    }
}

void P_ShutdownWindow(Window* const window)
{
    SDL_DestroyWindow(window->sdlWindow);
}