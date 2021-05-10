#pragma once

namespace DXP
{

struct DirectX11RenderTarget : public virtual RenderTarget
{
    virtual ID3D11RenderTargetView* GetRenderTarget() const = 0;

    virtual void Resize(ID3D11Device* device, ID3D11DeviceContext* context, int width, int height) = 0;
};

struct DirectX11BackbufferRenderTarget : public DirectX11RenderTarget
{
    DirectX11BackbufferRenderTarget(const Microsoft::WRL::ComPtr<IDXGISwapChain>& swapchain, const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& backbuffer) :
        swapchain(swapchain),
        backbuffer(backbuffer)
    {
    }

    virtual ID3D11RenderTargetView* GetRenderTarget() const {
        return backbuffer.Get();
    };

    virtual void Resize(ID3D11Device* device, ID3D11DeviceContext* context, int width, int height);

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbuffer;
};

};
