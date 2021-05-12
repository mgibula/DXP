#include "pch.h"

namespace DXP
{

void DirectX11BackbufferRenderTarget::Resize(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height)
{
    backbuffer->Release();
    swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    ID3D11Texture2D* pBackBuffer;
    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        Fatal("swapchain::GetBuffer");

    // Create render target
    if (FAILED(device->CreateRenderTargetView(pBackBuffer, nullptr, backbuffer.GetAddressOf())))
        Fatal("CreateRenderTargetView");

    pBackBuffer->Release();

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 1.f;

    context->RSSetViewports(1, &viewport);
}

};
