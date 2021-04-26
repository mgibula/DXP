#pragma once

namespace DXP
{

struct DirectX11VertexBuffer : public VertexBuffer
{
    DirectX11VertexBuffer(const BufferBase* buffer, ID3D11Device* device);
    virtual ~DirectX11VertexBuffer() = default;

    UINT stride = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ptr;
};

struct DirectX11IndexBuffer : public IndexBuffer
{
    DirectX11IndexBuffer(const BufferBase* buffer, ID3D11Device* device);
    virtual ~DirectX11IndexBuffer() = default;

    BufferFormat format;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ptr;
};

struct DirectX11ConstantBuffer : public ConstantBuffer
{
    DirectX11ConstantBuffer(size_t size, ID3D11Device* device);
    virtual ~DirectX11ConstantBuffer() = default;

    void Update(const void* data, size_t size, ID3D11DeviceContext* context);

    size_t size;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ptr;
};

};
