#ifndef GAME_H
#define GAME_H

#include <platform/window.h>

typedef struct
{
    Window* const window;
} Game;

void G_StartGame();

#endif