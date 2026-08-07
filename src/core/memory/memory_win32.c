#include "memory.h"

#include <windows.h>
#include <winnt.h>

void* M_MemAlloc(const u32 size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void M_MemFree(void* const addr)
{
    VirtualFree(addr, 0, MEM_RELEASE);
}