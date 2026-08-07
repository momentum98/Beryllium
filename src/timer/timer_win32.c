#include "timer.h"

#include <profileapi.h>
#include <windows.h>
#include <winnt.h>

void T_SetupTimer(Timer* const timer)
{
    QueryPerformanceFrequency((LARGE_INTEGER*) &timer->frequency);

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    timer->lastCounter = counter.QuadPart;
}

void T_UpdateTimer(Timer* const timer)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter((LARGE_INTEGER*) &counter);

    const u64 updates = counter.QuadPart - timer->lastCounter;

    timer->deltaTime = (f32) updates / (f32) timer->frequency;
    timer->currTime += timer->deltaTime;

    timer->lastCounter = counter.QuadPart;
}