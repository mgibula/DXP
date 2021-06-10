#pragma once

namespace DXP
{

struct DirectX11DepthStencilTest : public DepthStencilTest
{
    DirectX11DepthStencilTest(ID3D11Device* device, const DepthStencilTestDescription& description);

    virtual DepthStencilTestDescription GetDescription() const override;

    virtual bool DepthTestEnabled() const override;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> state;

    DepthStencilTestDescription description;
};

};
