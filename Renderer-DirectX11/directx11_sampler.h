#pragma once

namespace DXP
{

struct DirectX11Sampler : public Sampler
{
    DirectX11Sampler(ID3D11Device* device, const SamplerDescription& settings);

    virtual SamplerDescription GetDescription() const override;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> ptr;

    SamplerDescription description;
};

};
