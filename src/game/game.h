#ifndef GAME_H
#define GAME_H

#include <platform/window.h>
#include <timer/timer.h>

typedef struct
{
    Window* const window;
    Timer* const timer;
} Game;

void G_StartGame();

#endif