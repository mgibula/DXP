#include "pch.h"

namespace DXP
{

static DXGI_FORMAT GetFormat(const TextureData& textureData);
static int GetTexelSize(const TextureData& textureData);

DirectX11Texture2D::DirectX11Texture2D(ID3D11Device* device, const TextureData& textureData)
{
    channels = textureData.loaded_channels;
    width = textureData.width;
    height = textureData.height;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = textureData.width;
    desc.Height = textureData.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = GetFormat(textureData);
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subresource = {};
    subresource.pSysMem = textureData.data.data();
    subresource.SysMemPitch = textureData.width * GetTexelSize(textureData) * textureData.loaded_channels;
    subresource.SysMemSlicePitch = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
    SetupShaderView(&view_desc, desc.Format);

    InitializeTexture(device, desc, &subresource);
    InitializeShaderView(device, view_desc);
}

void DirectX11Texture2D::SetupShaderView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc, DXGI_FORMAT format)
{
    desc->Format = format;
    desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc->Texture2D.MipLevels = 1;
    desc->Texture2D.MostDetailedMip = 0;
}

void DirectX11Texture2D::InitializeTexture(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA *subresource)
{
    HRESULT success = device->CreateTexture2D(&desc, subresource, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateTexture2D");
}

void DirectX11Texture2D::InitializeShaderView(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
{
    HRESULT success = device->CreateShaderResourceView(ptr.Get(), &desc, view.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateShaderResourceView");
}

DirectX11RenderTexture::DirectX11RenderTexture(ID3D11Device* device, int width, int height)
{
    D3D11_TEXTURE2D_DESC desc = {};
    SetupRenderTexture(&desc, width, height);
    InitializeTexture(device, desc, nullptr);

    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
    SetupShaderView(&view_desc, desc.Format);
    InitializeShaderView(device, view_desc);

    HRESULT success = device->CreateRenderTargetView(ptr.Get(), nullptr, renderTarget.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateRenderTargetView");
}

void DirectX11RenderTexture::SetupRenderTexture(D3D11_TEXTURE2D_DESC* desc, int width, int height)
{
    desc->Width = width;
    desc->Height = height;
    desc->MipLevels = 1;
    desc->ArraySize = 1;
    desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc->SampleDesc.Count = 1;
    desc->Usage = D3D11_USAGE_DEFAULT;
    desc->BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc->CPUAccessFlags = 0;

    channels = 4;
    this->width = width;
    this->height = height;
}

void DirectX11RenderTexture::Resize(ID3D11Device* device, ID3D11DeviceContext* context, int width, int height)
{
    ptr.Reset();
    view.Reset();
    renderTarget.Reset();

    D3D11_TEXTURE2D_DESC desc = {};
    SetupRenderTexture(&desc, width, height);
    InitializeTexture(device, desc, nullptr);

    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
    SetupShaderView(&view_desc, desc.Format);
    InitializeShaderView(device, view_desc);

    HRESULT success = device->CreateRenderTargetView(ptr.Get(), nullptr, renderTarget.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateRenderTargetView");
}

DirectX11DepthStencilTexture::DirectX11DepthStencilTexture(ID3D11Device* device, int width, int height)
{
    channels = 1;
    this->width = width;
    this->height = height;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    //desc.Format = DXGI_FORMAT_R24G8_TYPELESS; //  DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    InitializeTexture(device, desc, nullptr);

    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
    SetupShaderView(&view_desc, /* DXGI_FORMAT_D24_UNORM_S8_UINT */ DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
    //InitializeShaderView(device, view_desc);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsv = {};
    dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    //dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsv.Texture2D.MipSlice = 0;

    HRESULT success = device->CreateDepthStencilView(ptr.Get(), &dsv, depthStencilView.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateDepthStencilView");
}

static int GetTexelSize(const TextureData& textureData)
{
    switch (textureData.texel_format) {
    case TexelFormat::UInt8:
        return sizeof(uint8_t);
    case TexelFormat::UInt16:
        return sizeof(uint16_t);
    case TexelFormat::Float32:
        return sizeof(float);
    default:
        DXP::Fatal("Unsupported texel format: {}", static_cast<int>(textureData.texel_format));
    }
}

static DXGI_FORMAT GetFormat(const TextureData& textureData)
{
    int channels = textureData.loaded_channels;

    if (textureData.texel_format == TexelFormat::UInt8) {
        switch (channels) {
        case 1: return DXGI_FORMAT_R8_UINT;
        case 2: return DXGI_FORMAT_R8G8_UINT;
        case 4: return DXGI_FORMAT_R8G8B8A8_UINT;
        }
    } else if (textureData.texel_format == TexelFormat::UInt16) {
        switch (channels) {
        case 1: return DXGI_FORMAT_R16_UINT;
        case 2: return DXGI_FORMAT_R16G16_UINT;
        case 4: return DXGI_FORMAT_R16G16B16A16_UINT;
        }

    } else if (textureData.texel_format == TexelFormat::Float32) {
        switch (channels) {
        case 1: return DXGI_FORMAT_R32_FLOAT;
        case 2: return DXGI_FORMAT_R32G32_FLOAT;
        case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

    }

    DXP::Fatal("Unsupported texel format: {} with {} channels", static_cast<int>(textureData.texel_format), channels);
}

};
