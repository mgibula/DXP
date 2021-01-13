#pragma once

namespace DXP
{

struct DirectX11VertexBuffer : public VertexBuffer
{
    DirectX11VertexBuffer(const BufferBase* buffer, ID3D11Device* device);
    virtual ~DirectX11VertexBuffer() = default;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ptr;
};

struct DirectX11IndexBuffer : public IndexBuffer
{
    DirectX11IndexBuffer(const BufferBase* buffer, ID3D11Device* device);
    virtual ~DirectX11IndexBuffer() = default;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ptr;
};

};
