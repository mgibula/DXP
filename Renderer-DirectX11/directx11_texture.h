#pragma once

namespace DXP
{

struct DirectX11Texture2D : public virtual Texture
{
    DirectX11Texture2D() = default;
    DirectX11Texture2D(ID3D11Device* device, const TextureData& settings);

    virtual int Channels() const override {
        return channels;
    };

    virtual uint32_t Width() const override {
        return width;
    };

    virtual uint32_t Height() const override {
        return height;
    };

    virtual void* GetImGuiImage() const override {
        return view.Get();
    };

    void InitializeTexture(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* subresource);

    void SetupShaderView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc, DXGI_FORMAT format);
    void InitializeShaderView(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc);

    int channels = 0;
    int width = 0;
    int height = 0;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> ptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;
};

struct DirectX11RenderTexture : public RenderTexture, public DirectX11Texture2D, public DirectX11RenderTarget
{
    DirectX11RenderTexture(ID3D11Device* device, int width, int height);

    virtual int Channels() const override {
        return DirectX11Texture2D::Channels();
    };

    virtual uint32_t Width() const override {
        return DirectX11Texture2D::Width();
    };

    virtual uint32_t Height() const override {
        return DirectX11Texture2D::Height();
    };

    virtual void* GetImGuiImage() const override {
        return DirectX11Texture2D::GetImGuiImage();
    };

    virtual ID3D11RenderTargetView* GetRenderTarget() const {
        return renderTarget.Get();
    };

    virtual void Resize(ID3D11Device* device, int width, int height);

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;

private:
    void SetupRenderTexture(D3D11_TEXTURE2D_DESC* desc, int width, int height);
};

struct DirectX11DepthStencilTexture : public DepthStencilTexture, public DirectX11Texture2D
{
    DirectX11DepthStencilTexture(ID3D11Device* device, int width, int height);

    virtual int Channels() const override {
        return DirectX11Texture2D::Channels();
    };

    virtual uint32_t Width() const override {
        return DirectX11Texture2D::Width();
    };

    virtual uint32_t Height() const override {
        return DirectX11Texture2D::Height();
    };

    virtual void* GetImGuiImage() const override {
        return DirectX11Texture2D::GetImGuiImage();
    };

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
};

};
