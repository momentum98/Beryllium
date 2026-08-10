#ifndef GAME_H
#define GAME_H

#include <managers/input/input_manager.h>
#include <backend/backend.h>
#include <platform/window.h>
#include <timer/timer.h>

typedef struct
{
    Window* const       window;
    Timer* const        timer;
    InputManager* const inputManager;
    Backend* const      backend;
} Game;

void G_StartGame();

#endif