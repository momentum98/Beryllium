#ifndef DX12_H
#define DX12_H

#include <platform/window.h>
#include <dxgi1_4.h>
#include <types.h>
#include <d3d12.h>
#include <dxgi.h>

typedef struct
{
    ID3D12Fence* fence;

    HANDLE       event;
    u32          progressCounter;
} DX12Fence;

typedef struct
{
    ID3D12Resource*             screenBuffers[2];
    D3D12_CPU_DESCRIPTOR_HANDLE screenBufferHandles[2];
    
    u32                         barrierBackBufferIndex;
} DX12ScreenBuffers;

typedef struct
{
    Window*                    window;

    IDXGIFactory4*             factory;
    IDXGISwapChain3*           swapChain;

    ID3D12Device*              device;

    ID3D12CommandQueue*        commandQueue;
    ID3D12CommandAllocator*    commandAllocator;
    ID3D12GraphicsCommandList* commandList;

    DX12Fence                  fence;

    ID3D12DescriptorHeap*      rtvDescriptorHeap;

    ID3D12Resource*            uploadBuffer;

    DX12ScreenBuffers          screenBuffers;
} DX12;

void B_DX12Setup(DX12* const dx12, const u32 width, const u32 height, void* handle);
void B_DX12ResizeScreenBuffers(DX12* const dx12, const u32 width, const u32 height);
void B_DX12NewFrame(DX12* const dx12, const f32 backColor[4]);
void B_DX12EndFrame(DX12* const dx12);
void B_DX12Shutdown(DX12* const dx12);

#endif