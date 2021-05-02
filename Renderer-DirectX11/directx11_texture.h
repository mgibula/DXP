#pragma once

namespace DXP
{

struct DirectX11Texture2D : public Texture
{
    DirectX11Texture2D(ID3D11Device* device, const TextureData& settings);

    virtual int Channels() const override {
        return channels;
    };

    virtual uint32_t Width() const override {
        return width;
    };

    virtual uint32_t Height() const override {
        return height;
    };

    int channels = 0;
    int width = 0;
    int height = 0;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> ptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;
};

};
