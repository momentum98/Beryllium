#ifndef BACKEND_H
#define BACKEND_H

#include <platform/window.h>
#include <types.h>

typedef enum
{
    DX12_BACKEND
} BackendType;

typedef struct
{
    BackendType type;

    union 
    {
        void* dx12;
    };
} Backend;

void B_BackendSetup(Backend* const backend, const u32 width, const u32 height, Window* const window);
void B_BackendResizeScreenBuffers(Backend* const backend, const u32 width, const u32 height);
void B_BackendNewFrame(Backend* const backend, const f32 backColor[3]);
void B_BackendEndFrame(Backend* const backend);
void B_BackendShutdown(Backend* const backend);

#endif