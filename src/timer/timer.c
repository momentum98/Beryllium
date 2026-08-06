#include "timer.h"
#include <profileapi.h>
#include <winnt.h>

void T_SetupTimer(Timer* const timer)
{
    QueryPerformanceFrequency(&timer->frequency);

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    timer->lastCounter = counter;
}

void T_UpdateTimer(Timer* const timer)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    const u64 updates = counter.QuadPart - timer->lastCounter.QuadPart;

    timer->deltaTime = (f32) updates / (f32) timer->frequency.QuadPart;
    timer->currTime += timer->deltaTime;

    timer->lastCounter = counter;
}