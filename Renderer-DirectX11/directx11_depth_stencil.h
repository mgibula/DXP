#pragma once

namespace DXP
{

struct DirectX11DepthStencilTest : public DepthStencilTest
{
    DirectX11DepthStencilTest(ID3D11Device* device, bool depthEnabled);

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> state;
};

};
