#include "pch.h"

namespace DXP
{

DirectX11DepthStencilTest::DirectX11DepthStencilTest(ID3D11Device* device, const DepthStencilTestDescription& description) :
    description(description)
{
    D3D11_DEPTH_STENCIL_DESC desc = {};

    desc.DepthEnable = description.depth;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS;

    device->CreateDepthStencilState(&desc, state.GetAddressOf());
}

bool DirectX11DepthStencilTest::DepthTestEnabled() const
{
    return description.depth;
}

DepthStencilTestDescription DirectX11DepthStencilTest::GetDescription() const
{
    return description;
}

};
