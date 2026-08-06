#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>

void* M_BufferOffset(u8* const buffer, const u32 offset);
void* M_MemAlloc(const u32 size);
void M_MemFree(void* const addr);

#endif