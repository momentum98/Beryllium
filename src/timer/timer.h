#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include <winnt.h>
#include <types.h>

typedef struct
{
    u64 frequency;
    u64 lastCounter;

    f32 currTime;

    f32 deltaTime;
} Timer;

void T_UpdateTimer(Timer* const timer);
void T_SetupTimer(Timer* const timer);

#endif