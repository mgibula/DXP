#include "pch.h"

namespace DXP
{

static DXGI_FORMAT GetFormat(const TextureData& textureData);
static int GetTexelSize(const TextureData& textureData);

DirectX11RenderTexture::DirectX11RenderTexture(ID3D11Device* device, int width, int height)
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    channels = 4;
    this->width = width;
    this->height = height;

    Initialize(device, desc, nullptr);

    HRESULT success = device->CreateRenderTargetView(ptr.Get(), nullptr, renderTarget.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateRenderTargetView");
}

DirectX11Texture2D::DirectX11Texture2D(ID3D11Device* device, const TextureData& textureData)
{
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

    channels = textureData.loaded_channels;
    width = textureData.width;
    height = textureData.height;

    Initialize(device, desc, &subresource);
}

void DirectX11Texture2D::Initialize(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA *subresource)
{
    HRESULT success = device->CreateTexture2D(&desc, subresource, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateTexture2D");

    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
    view_desc.Format = desc.Format;
    view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    view_desc.Texture2D.MipLevels = 1;
    view_desc.Texture2D.MostDetailedMip = 0;

    success = device->CreateShaderResourceView(ptr.Get(), &view_desc, view.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateShaderResourceView");
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
