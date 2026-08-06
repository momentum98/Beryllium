#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>

static inline void* M_BufferOffset(u8* const buffer, const i32 dataSize, const i32 count)
{
    return (void*) (buffer + (dataSize * count));
}

void* M_MemAlloc(const u32 size);
void M_MemFree(void* const addr);

#endif