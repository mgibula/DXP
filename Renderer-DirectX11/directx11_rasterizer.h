#pragma once

namespace DXP
{

struct DirectX11Rasterizer : public Rasterizer
{
    DirectX11Rasterizer(ID3D11Device* device, const RasterizerDescription& description);

    virtual RasterizerDescription GetDescription() const override;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> ptr;

    RasterizerDescription description;
};

};
