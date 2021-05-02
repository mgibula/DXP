#include "pch.h"

namespace DXP
{

static D3D11_FILTER GetPointSamplerFilterType(const SamplerSettings& settings);
static D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(SamplerWrap mode);

DirectX11Sampler::DirectX11Sampler(ID3D11Device* device, const SamplerSettings& settings)
{
    D3D11_SAMPLER_DESC desc = {};

    switch (settings.type) {
    case SamplerType::Point:
        desc.Filter = GetPointSamplerFilterType(settings);
        break;
    case SamplerType::Anisotropic:
        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        desc.MaxAnisotropy = settings.anisotropic.level;
        break;
    }
    desc.AddressU = GetAddressMode(settings.wrapping.U);
    desc.AddressV = GetAddressMode(settings.wrapping.V);
    desc.AddressW = GetAddressMode(settings.wrapping.W);

    desc.MipLODBias = settings.lod.bias;
    desc.MinLOD = settings.lod.min;
    desc.MaxLOD = settings.lod.max;

    memcpy(desc.BorderColor, settings.border, sizeof(float) * 4);

    HRESULT created = device->CreateSamplerState(&desc, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(created), "CreateSamplerState");
}

static D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(SamplerWrap mode)
{
    switch (mode) {
    case SamplerWrap::Wrap:
        return D3D11_TEXTURE_ADDRESS_WRAP;
    case SamplerWrap::Mirror:
        return D3D11_TEXTURE_ADDRESS_MIRROR;
    case SamplerWrap::Clamp:
        return D3D11_TEXTURE_ADDRESS_CLAMP;
    case SamplerWrap::Border:
        return D3D11_TEXTURE_ADDRESS_BORDER;
    default:
        DXP::Fatal("Unknown sampler wrap mode: {}", static_cast<int>(mode));
    }
}

static D3D11_FILTER GetPointSamplerFilterType(const SamplerSettings& settings)
{
    bool min = settings.point.minification_linear;
    bool mag = settings.point.magnification_linear;
    bool mip = settings.point.mip_linear;

    if (!min && !mag && !mip)
        return D3D11_FILTER_MIN_MAG_MIP_POINT;

    if (!min && !mag && mip)
        return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

    if (!min && mag && !mip)
        return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

    if (!min && mag && mip)
        return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

    if (min && !mag && !mip)
        return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

    if (min && !mag && mip)
        return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

    if (min && mag && !mip)
        return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

    if (min && mag && mip)
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    // Shouldn't be really possible
    DXP::Fatal("Unknown sampler filter type: {}/{}/{}", min, mag, mip);
}

};
