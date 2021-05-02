#pragma once

namespace DXP
{

struct DirectX11Sampler : public Sampler
{
    DirectX11Sampler(ID3D11Device* device, const SamplerSettings& settings);

    Microsoft::WRL::ComPtr<ID3D11SamplerState> ptr;
};

};
