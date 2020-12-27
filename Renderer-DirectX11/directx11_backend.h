#pragma once
#include "../Engine/render_backend.h"
#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace DXP
{

struct DirectX11Backend final : public RenderBackend
{
    DirectX11Backend(HWND window);

    virtual bool Initialize() override;

private:
    HWND window;
    int width = 0;
    int height = 0;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbuffer;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;
};

};
