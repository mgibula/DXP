#pragma once
#include "../Engine/render_backend.h"

namespace DXP
{

struct DirectX11Backend final : public RenderBackend
{
    DirectX11Backend(HWND window);

    virtual bool Initialize() override;
    virtual void PreRenderLoop() override;
    virtual void PostRenderLoop() override;
    virtual void OnFrameStart() override;
    virtual void OnFrameEnd() override;

    virtual void ImGuiInit() override;
    virtual void ImGuiShutdown() override;
    virtual void ImGuiFrameStart() override;
    virtual void ImGuiFrameEnd() override;

    virtual void ClearScreen() override;
    virtual void Display() override;

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
