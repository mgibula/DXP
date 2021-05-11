#include "pch.h"

namespace DXP
{

DirectX11DepthStencilTest::DirectX11DepthStencilTest(ID3D11Device* device, bool depthEnabled)
{
    D3D11_DEPTH_STENCIL_DESC desc = {};

    desc.DepthEnable = true;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS;

    device->CreateDepthStencilState(&desc, state.GetAddressOf());
}

};
