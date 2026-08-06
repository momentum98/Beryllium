#include "game.h"

#include <SDL.h>
#include <platform/window.h>

#define DEFAULT_WINDOW_TITLE "Beryllium 1.0.0 (Proxi)"

#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 600

void Tick(Game game)
{
}

void EndGame(Game game)
{
    SDL_Quit();
}

void G_StartGame()
{
    SDL_Init(SDL_INIT_VIDEO);
    
    u32 res = 0;
    
    Window window = { 0 };
    res = P_SetupWindow(&window, DEFAULT_WINDOW_TITLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    if (res > 0)
        EndGame(
            (Game) { &window }
        );

    while (window.isRunning)
    {
        P_UpdateWindow(&window);
        
        Tick(
            (Game) { &window }
        );
    }

    EndGame(
        (Game) { &window }
    );
}