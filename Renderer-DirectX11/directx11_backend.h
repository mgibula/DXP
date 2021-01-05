#pragma once

namespace DXP
{

struct DirectX11Backend final : public RenderBackend
{
    DirectX11Backend(HWND window, std::shared_ptr<spdlog::logger> log);

    virtual std::string InfoString() const override;

    virtual bool Initialize() override;
    virtual void PreRenderLoop(Engine* engine) override;
    virtual void PostRenderLoop(Engine* engine) override;
    virtual void OnFrameStart(Engine* engine) override;
    virtual void OnFrameEnd(Engine* engine) override;

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
    std::shared_ptr<spdlog::logger> log;
};

};
