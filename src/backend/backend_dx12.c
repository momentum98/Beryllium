#include "backend.h"

#include <core/memory/memory.h>
#include <backend/dx12/dx12.h>

void B_BackendSetup(Backend* const backend, const u32 width, const u32 height, Window* const window)
{
    backend->dx12 = M_MemAlloc(sizeof(DX12));
    ((DX12*) backend->dx12)->window = window;
    
    B_DX12Setup((DX12*) backend->dx12, width, height, window->handle);
}

void B_BackendNewFrame(Backend* const backend, const f32 backColor[4])
{
    B_DX12NewFrame((DX12*) backend->dx12, backColor);
}

void B_BackendEndFrame(Backend* const backend)
{
    B_DX12EndFrame((DX12*) backend->dx12);
}

void B_BackendResizeScreenBuffers(Backend* const backend, const u32 width, const u32 height)
{
    B_DX12ResizeScreenBuffers(backend->dx12, width, height);
}

void B_BackendShutdown(Backend* const backend)
{
    B_DX12Shutdown((DX12*) backend->dx12);
}