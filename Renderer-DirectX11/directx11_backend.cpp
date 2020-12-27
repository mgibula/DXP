#include "directx11_backend.h"
#include "../Engine/utils.h"

#pragma comment(lib, "d3d11.lib")

namespace DXP
{

DirectX11Backend::DirectX11Backend(HWND window) :
    window(window)
{
}

bool DirectX11Backend::Initialize()
{
    DXGI_SWAP_CHAIN_DESC scd = {};

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = window;
    scd.SampleDesc.Count = 1;
    scd.Windowed = true;

    // Create device, context and swapchain
    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        swapchain.GetAddressOf(),
        device.GetAddressOf(),
        nullptr,
        context.GetAddressOf()
    );

    if (FAILED(result))
        Fatal("D3D11CreateDeviceAndSwapChain");

    // Get backbuffer
    ID3D11Texture2D* pBackBuffer;
    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        Fatal("swapchain::GetBuffer");

    // Create render target
    if (FAILED(device->CreateRenderTargetView(pBackBuffer, nullptr, backbuffer.GetAddressOf())))
        Fatal("CreateRenderTargetView");

    pBackBuffer->Release();

    // Set render target
    context->OMSetRenderTargets(1, backbuffer.GetAddressOf(), nullptr);

    // Viewport setup
    RECT rect;
    if (!GetWindowRect(window, &rect))
        Fatal("GetWindowRect");

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;

    // Rasterizer states
    D3D11_RASTERIZER_DESC solid = {};
    solid.FillMode = D3D11_FILL_SOLID;
    solid.CullMode = D3D11_CULL_BACK;

    device->CreateRasterizerState(&solid, rasterizer.GetAddressOf());

    context->RSSetState(rasterizer.Get());
    context->RSSetViewports(1, &viewport);

    return true;
}

void DirectX11Backend::ClearScreen()
{
    float black[4] = {};
    context->ClearRenderTargetView(backbuffer.Get(), black);
}

void DirectX11Backend::Display()
{
    swapchain->Present(0, 0);
}

};
