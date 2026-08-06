#include "game.h"

#include <core/memory/memory.h>
#include <platform/window.h>
#include <logger/logger.h>
#include <timer/timer.h>
#include <SDL.h>

#define DEFAULT_WINDOW_TITLE "Beryllium 1.0.0 (Proxi)"

#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 600

void Tick(Game game)
{
    // L_LogInfo("Delta Time: %.5f", game.timer->deltaTime);
    // L_LogInfo("Current Time: %.2f", game.timer->currTime);
}

void EndGame(Game game)
{
    L_LogInfo("Engine shutting down...");

    SDL_Quit();

    M_MemFree(game.window->screenBuffer.buffer);
    game.window->screenBuffer.buffer = NULL;

    M_MemFree(game.inputManager->keybinds);
    game.inputManager->keybindCount = 0;
    game.inputManager->keybinds = NULL;
}

void G_StartGame()
{
    L_LogInfo("Engine started...");
    SDL_Init(SDL_INIT_VIDEO);
    
    u32 res = 0;

    Timer timer = { 0 };
    T_SetupTimer(&timer);
    
    Window window = { 0 };
    res = P_SetupWindow(&window, DEFAULT_WINDOW_TITLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    InputManager inputManager = { 0 };
    IM_SetupInputManager(&inputManager);

    if (res > 0)
    {
        L_LogError("Failed to setup window...");
        
        EndGame(
            (Game) { &window, &timer, &inputManager }
        );
    }

    while (window.isRunning)
    {
        T_UpdateTimer(&timer);
        P_UpdateWindow(&window, &inputManager);
        
        Tick(
            (Game) { &window, &timer, &inputManager }
        );
    }

    EndGame(
        (Game) { &window, &timer, &inputManager }
    );
}