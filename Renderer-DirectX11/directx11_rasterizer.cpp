#include "pch.h"

namespace DXP
{

DirectX11Rasterizer::DirectX11Rasterizer(ID3D11Device* device, const RasterizerSettings& settings)
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.DepthClipEnable = true;

    if (settings.wireframe) {
        desc.FillMode = D3D11_FILL_WIREFRAME;
    } else {
        desc.FillMode = D3D11_FILL_SOLID;
    }

    if (settings.drawFront && settings.drawBack) {
        desc.CullMode = D3D11_CULL_NONE;
    } else if (settings.drawFront) {
        desc.CullMode = D3D11_CULL_BACK;
    } else if (settings.drawBack) {
        desc.CullMode = D3D11_CULL_FRONT;
    } else {
        DXP::Fatal("Unsupported culling settings");
    }

    device->CreateRasterizerState(&desc, ptr.GetAddressOf());
}

};
