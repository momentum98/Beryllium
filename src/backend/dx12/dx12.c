#include "dx12.h"

#include <dxgi1_4.h>
#include <d3d12.h>
#include <dxgiformat.h>
#include <types.h>
#include <dxgi.h>

static void DX12CreateSwapChain(DX12* const dx12, const u32 width, const u32 height, void* handle)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SampleDesc.Count = 1;

    IDXGISwapChain1* swapChain1 = NULL;

    const HRESULT res = dx12->factory->lpVtbl->CreateSwapChainForHwnd(
        dx12->factory, 
        (IUnknown*) dx12->commandQueue, 
        (HWND) handle, 
        &swapChainDesc, 
        NULL, 
        NULL, 
        &swapChain1
    );
    swapChain1->lpVtbl->QueryInterface(swapChain1, &IID_IDXGISwapChain3, (void**) &dx12->swapChain);

    swapChain1->lpVtbl->Release(swapChain1);
}

static void DX12CreateCommandQueue(DX12* const dx12, const u32 width, const u32 height, void* handle)
{
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = { 0 };
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandQueueDesc.NodeMask = 0;

    dx12->device->lpVtbl->CreateCommandQueue(
        dx12->device, 
        &commandQueueDesc, 
        &IID_ID3D12CommandQueue, 
        (void**) &dx12->commandQueue
    );
}

static void DX12FenceWait(DX12* const dx12)
{
    ++dx12->fence.progressCounter;
    dx12->commandQueue->lpVtbl->Signal(dx12->commandQueue, dx12->fence.fence, dx12->fence.progressCounter);

    if (dx12->fence.fence->lpVtbl->GetCompletedValue(dx12->fence.fence) < dx12->fence.progressCounter)
    {
        dx12->fence.fence->lpVtbl->SetEventOnCompletion(dx12->fence.fence, dx12->fence.progressCounter, dx12->fence.event);
        WaitForSingleObject(dx12->fence.event, INFINITE);
    }
}

static void DX12GetBufferHandles(DX12* const dx12, D3D12_CPU_DESCRIPTOR_HANDLE* const handles)
{
    const u32 rtvDescriptorSize = dx12->device->lpVtbl->GetDescriptorHandleIncrementSize(
        dx12->device, 
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV
    );
    
    D3D12_CPU_DESCRIPTOR_HANDLE rtvCPUHandle = { 0 };
    dx12->rtvDescriptorHeap->lpVtbl->GetCPUDescriptorHandleForHeapStart(dx12->rtvDescriptorHeap, &rtvCPUHandle);
    
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvCPUHandle;

    handles[0] = rtvCPUHandle;
    
    handles[1] = rtvCPUHandle;
    handles[1].ptr += rtvDescriptorSize;
}

void DX12ClearBackBuffer(DX12* const dx12, const D3D12_CPU_DESCRIPTOR_HANDLE backBufferHandle, const f32 backColor[4])
{
    D3D12_RECT rect = { 0 };
    rect.bottom = 0;
    rect.top = 0;
    rect.right = dx12->window->screenBuffer.width;
    rect.bottom = dx12->window->screenBuffer.height;
    
    dx12->commandList->lpVtbl->ClearRenderTargetView(
        dx12->commandList, 
        backBufferHandle, 
        backColor, 
        1, 
        &rect
    );
}

static void DX12CreateRTVDescriptorHeap(DX12* const dx12)
{
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = { 0 };
    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    descriptorHeapDesc.NumDescriptors = 2;
    descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descriptorHeapDesc.NodeMask = 0;

    dx12->device->lpVtbl->CreateDescriptorHeap(
        dx12->device, 
        &descriptorHeapDesc, 
        &IID_ID3D12DescriptorHeap, 
        (void**) &dx12->rtvDescriptorHeap
    );
}

static void DX12SetViewport(DX12* const dx12)
{
    D3D12_RECT rect = { 0 };
    rect.left = 0;
    rect.top = 0;
    rect.right = dx12->window->screenBuffer.width;
    rect.bottom = dx12->window->screenBuffer.height;

    D3D12_VIEWPORT viewport = { 0 };
    viewport.Height = dx12->window->screenBuffer.height;
    viewport.Width = dx12->window->screenBuffer.width;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    dx12->commandList->lpVtbl->RSSetScissorRects(dx12->commandList, 1, &rect);
    dx12->commandList->lpVtbl->RSSetViewports(dx12->commandList, 1, &viewport);
}

static void DX12SetupScreenBuffers(DX12* const dx12)
{
    dx12->swapChain->lpVtbl->GetBuffer(
        dx12->swapChain, 
        0, 
        &IID_ID3D12Resource, 
        (void**) &dx12->screenBuffers.screenBuffers[0]
    );
    dx12->swapChain->lpVtbl->GetBuffer(
        dx12->swapChain, 
        1, 
        &IID_ID3D12Resource, 
        (void**) &dx12->screenBuffers.screenBuffers[1]
    );

    dx12->device->lpVtbl->CreateRenderTargetView(
        dx12->device, 
        dx12->screenBuffers.screenBuffers[0], 
        NULL, 
        dx12->screenBuffers.screenBufferHandles[0]
    );
    dx12->device->lpVtbl->CreateRenderTargetView(
        dx12->device, 
        dx12->screenBuffers.screenBuffers[1], 
        NULL, 
        dx12->screenBuffers.screenBufferHandles[1]
    );
}

void B_DX12Setup(DX12* const dx12, const u32 width, const u32 height, void* handle)
{
    CreateDXGIFactory1(&IID_IDXGIFactory4, (void*) &dx12->factory);
    D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, &IID_ID3D12Device, (void**) &dx12->device);
    
    DX12CreateCommandQueue(dx12, width, height, handle);
    DX12CreateSwapChain(dx12, width, height, handle);
    
    dx12->device->lpVtbl->CreateCommandAllocator(
        dx12->device, 
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        &IID_ID3D12CommandAllocator, 
        (void**) &dx12->commandAllocator
    );
    dx12->device->lpVtbl->CreateCommandList(
        dx12->device, 
        0, 
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        dx12->commandAllocator, 
        NULL, 
        &IID_ID3D12CommandList, 
        (void**) &dx12->commandList
    );
    dx12->commandList->lpVtbl->Close(dx12->commandList);

    dx12->fence.event = CreateEvent(NULL, true, false, NULL);

    DX12CreateRTVDescriptorHeap(dx12); 
    DX12GetBufferHandles(dx12, dx12->screenBuffers.screenBufferHandles);

    DX12SetupScreenBuffers(dx12);

    dx12->device->lpVtbl->CreateFence(dx12->device, 0, 0, &IID_ID3D12Fence, (void**) &dx12->fence.fence);
}

void B_DX12NewFrame(DX12* const dx12, const f32 backColor[4])
{
    dx12->commandAllocator->lpVtbl->Reset(dx12->commandAllocator);
    dx12->commandList->lpVtbl->Reset(dx12->commandList, dx12->commandAllocator, NULL);
    
    const u32 backBufferIndex = dx12->swapChain->lpVtbl->GetCurrentBackBufferIndex(dx12->swapChain);

    D3D12_RESOURCE_BARRIER flipTransitionBarrier = { 0 };
    flipTransitionBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    flipTransitionBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    flipTransitionBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    flipTransitionBarrier.Transition.pResource = dx12->screenBuffers.screenBuffers[backBufferIndex];

    dx12->commandList->lpVtbl->ResourceBarrier(dx12->commandList, 1, &flipTransitionBarrier);
    dx12->screenBuffers.barrierBackBufferIndex = backBufferIndex;

    const D3D12_CPU_DESCRIPTOR_HANDLE backBufferHandle = dx12->screenBuffers.screenBufferHandles[backBufferIndex];
    
    dx12->commandList->lpVtbl->OMSetRenderTargets(dx12->commandList, 1, &backBufferHandle, false, NULL);
    DX12ClearBackBuffer(dx12, backBufferHandle, backColor);
    DX12SetViewport(dx12);
}

void B_DX12EndFrame(DX12* const dx12)
{
    D3D12_RESOURCE_BARRIER flipTransitionBarrier = { 0 };
    flipTransitionBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    flipTransitionBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    flipTransitionBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    flipTransitionBarrier.Transition.pResource = dx12->screenBuffers.screenBuffers[dx12->screenBuffers.barrierBackBufferIndex];
    
    dx12->commandList->lpVtbl->ResourceBarrier(dx12->commandList, 1, &flipTransitionBarrier);

    dx12->commandList->lpVtbl->Close(dx12->commandList);

    ID3D12CommandList* const commandLists[1] = { (ID3D12CommandList*) dx12->commandList };
    dx12->commandQueue->lpVtbl->ExecuteCommandLists(dx12->commandQueue, 1, commandLists);

    dx12->swapChain->lpVtbl->Present(dx12->swapChain, 0, 0);
    DX12FenceWait(dx12);
}

void B_DX12ResizeScreenBuffers(DX12* const dx12, const u32 width, const u32 height)
{
    if (dx12->screenBuffers.screenBuffers[0] != NULL)
    {
        dx12->screenBuffers.screenBuffers[0]->lpVtbl->Release(dx12->screenBuffers.screenBuffers[0]);
        dx12->screenBuffers.screenBuffers[0] = NULL;
    }
    
    if (dx12->screenBuffers.screenBuffers[1] != NULL)
    {
        dx12->screenBuffers.screenBuffers[1]->lpVtbl->Release(dx12->screenBuffers.screenBuffers[1]);
        dx12->screenBuffers.screenBuffers[1] = NULL;
    }
    
    dx12->swapChain->lpVtbl->ResizeBuffers(dx12->swapChain, 2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    DX12SetupScreenBuffers(dx12);
}

void B_DX12Shutdown(DX12* const dx12)
{
    if (dx12->commandQueue != NULL && dx12->fence.fence != NULL && dx12->fence.event != NULL)
    {
        DX12FenceWait(dx12);
    }

    if (dx12->fence.event != NULL)
    {
        CloseHandle(dx12->fence.event);
        dx12->fence.event = NULL;
    }

    if (dx12->screenBuffers.screenBuffers[0] != NULL)
    {
        dx12->screenBuffers.screenBuffers[0]->lpVtbl->Release(dx12->screenBuffers.screenBuffers[0]);
        dx12->screenBuffers.screenBuffers[0] = NULL;
    }
    
    if (dx12->screenBuffers.screenBuffers[1] != NULL)
    {
        dx12->screenBuffers.screenBuffers[1]->lpVtbl->Release(dx12->screenBuffers.screenBuffers[1]);
        dx12->screenBuffers.screenBuffers[1] = NULL;
    }

    if (dx12->fence.fence != NULL)
    {
        dx12->fence.fence->lpVtbl->Release(dx12->fence.fence);
        dx12->fence.fence = NULL;
    }

    if (dx12->rtvDescriptorHeap != NULL)
    {
        dx12->rtvDescriptorHeap->lpVtbl->Release(dx12->rtvDescriptorHeap);
        dx12->rtvDescriptorHeap = NULL;
    }

    if (dx12->commandList != NULL)
    {
        dx12->commandList->lpVtbl->Release(dx12->commandList);
        dx12->commandList = NULL;
    }

    if (dx12->commandAllocator != NULL)
    {
        dx12->commandAllocator->lpVtbl->Release(dx12->commandAllocator);
        dx12->commandAllocator = NULL;
    }

    if (dx12->swapChain)
    {
        dx12->swapChain->lpVtbl->Release(dx12->swapChain);
        dx12->swapChain = NULL;
    }

    if (dx12->commandQueue)
    {
        dx12->commandQueue->lpVtbl->Release(dx12->commandQueue);
        dx12->commandQueue = NULL;
    }

    if (dx12->device)
    {
        dx12->device->lpVtbl->Release(dx12->device);
        dx12->device = NULL;
    }

    if (dx12->factory)
    {
        dx12->factory->lpVtbl->Release(dx12->factory);
        dx12->factory = NULL;
    }
}