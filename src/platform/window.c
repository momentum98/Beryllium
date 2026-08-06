#include "window.h"

#include "SDL_events.h"
#include "SDL_properties.h"

u32 P_SetupWindow(Window* const window, const char* title, const i32 width, const i32 height)
{
    SDL_Window* const sdlWindow = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);

    if (sdlWindow == NULL)
        return 1;

    HWND hWnd = SDL_GetPointerProperty(
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
            window->width = event.window.data1;
            window->height = event.window.data2;
        }
    }
}

void P_ShutdownWindow(Window* const window)
{
    SDL_DestroyWindow(window->sdlWindow);
}