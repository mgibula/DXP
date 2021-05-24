#include "pch.h"

namespace DXP
{

void DirectX11BackbufferRenderTarget::Resize(ID3D11Device* device, int width, int height)
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
}

};
