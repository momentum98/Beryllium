#ifndef GAME_H
#define GAME_H

#include <managers/input/input_manager.h>
#include <platform/window.h>
#include <timer/timer.h>

typedef struct
{
    Window* const       window;
    Timer* const        timer;
    InputManager* const inputManager;
} Game;

void G_StartGame();

#endif