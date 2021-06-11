#include "pch.h"

namespace DXP
{

static D3D11_FILTER GetPointSamplerFilterType(const SamplerDescription& settings);
static D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(SamplerWrap mode);

DirectX11Sampler::DirectX11Sampler(ID3D11Device* device, const SamplerDescription& description) :
    description(description)
{
    D3D11_SAMPLER_DESC desc = {};

    switch (description.type) {
    case SamplerType::Point:
        desc.Filter = GetPointSamplerFilterType(description);
        break;
    case SamplerType::Anisotropic:
        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        desc.MaxAnisotropy = description.anisotropic.level;
        break;
    }
    desc.AddressU = GetAddressMode(description.wrapping.U);
    desc.AddressV = GetAddressMode(description.wrapping.V);
    desc.AddressW = GetAddressMode(description.wrapping.W);

    desc.MipLODBias = description.lod.bias;
    desc.MinLOD = description.lod.min;
    desc.MaxLOD = description.lod.max;

    memcpy(desc.BorderColor, description.border, sizeof(float) * 4);

    HRESULT created = device->CreateSamplerState(&desc, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(created), "CreateSamplerState");
}

SamplerDescription DirectX11Sampler::GetDescription() const
{
    return description;
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

static D3D11_FILTER GetPointSamplerFilterType(const SamplerDescription& description)
{
    bool min = description.point.minification_linear;
    bool mag = description.point.magnification_linear;
    bool mip = description.point.mip_linear;

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
