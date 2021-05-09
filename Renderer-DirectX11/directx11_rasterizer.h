#pragma once

namespace DXP
{

struct DirectX11Rasterizer : public Rasterizer
{
    DirectX11Rasterizer(ID3D11Device* device, const RasterizerSettings& settings);

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> ptr;
};

};
