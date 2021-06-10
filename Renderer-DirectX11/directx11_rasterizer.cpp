#include "pch.h"

namespace DXP
{

DirectX11Rasterizer::DirectX11Rasterizer(ID3D11Device* device, const RasterizerDescription& description) :
    description(description)
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.DepthClipEnable = true;

    if (description.wireframe) {
        desc.FillMode = D3D11_FILL_WIREFRAME;
    } else {
        desc.FillMode = D3D11_FILL_SOLID;
    }

    if (description.drawFront && description.drawBack) {
        desc.CullMode = D3D11_CULL_NONE;
    } else if (description.drawFront) {
        desc.CullMode = D3D11_CULL_BACK;
    } else if (description.drawBack) {
        desc.CullMode = D3D11_CULL_FRONT;
    } else {
        DXP::Fatal("Unsupported culling settings");
    }

    device->CreateRasterizerState(&desc, ptr.GetAddressOf());
}

RasterizerDescription DirectX11Rasterizer::GetDescription() const
{
    return description;
}

};
