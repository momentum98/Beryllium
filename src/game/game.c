#include "game.h"

#include <managers/input/input_manager.h>
#include <core/memory/memory.h>
#include <platform/window.h>
#include <backend/backend.h>
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

    if (game.window != NULL)
        P_ShutdownWindow(game.window);

    if (game.inputManager != NULL)
        IM_ShutdownInputManager(game.inputManager);

    if (game.backend != NULL)
        B_BackendShutdown(game.backend);
}

void G_StartGame()
{
    L_LogInfo("Engine started...");
    
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        L_LogError("SDL_Init failed: %s", SDL_GetError());
        return;
    }
    
    u32 res = 0;

    Timer timer = { 0 };
    T_SetupTimer(&timer);
    
    Window window = { 0 };
    res = P_SetupWindow(&window, DEFAULT_WINDOW_TITLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    if (res > 0)
    {
        L_LogError("Failed to setup Window...");
        
        EndGame(
            (Game) { &window, &timer, NULL }
        );
        return;
    }

    InputManager inputManager = { 0 };
    res = IM_SetupInputManager(&inputManager);

    if (res > 0)
    {
        L_LogError("Failed to setup Input Manager...");
        
        EndGame(
            (Game) { &window, &timer, &inputManager }
        );
        return;
    }

    Backend backend = { 0 };
    backend.type = DX12_BACKEND;

    B_BackendSetup(&backend, window.screenBuffer.width, window.screenBuffer.height, &window);

    window.backend = &backend;
    
    while (window.isRunning)
    {
        T_UpdateTimer(&timer);
        res = P_UpdateWindow(&window, &inputManager);

        if (res > 0)
        {
            L_LogError("Failed to update Window...");
        
            EndGame(
                (Game) { &window, &timer, &inputManager }
            );
            return;
        }

        B_BackendNewFrame(&backend, (f32[]) { 0, 0, 0, 255 });
        
        Tick(
            (Game) { &window, &timer, &inputManager, &backend }
        );

        B_BackendEndFrame(&backend);
    }

    EndGame(
        (Game) { &window, &timer, &inputManager, &backend }
    );
}