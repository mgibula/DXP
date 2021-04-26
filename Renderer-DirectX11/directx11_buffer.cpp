#include "pch.h"

namespace DXP
{

DirectX11VertexBuffer::DirectX11VertexBuffer(const BufferBase* buffer, ID3D11Device* device) :
    stride(buffer->Stride())
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = static_cast<UINT>(buffer->GetBufferSize());
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resource = {};
    resource.pSysMem = buffer->GetBufferPtr();

    HRESULT created = device->CreateBuffer(&bd, &resource, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(created), "CreateBuffer");
}

DirectX11IndexBuffer::DirectX11IndexBuffer(const BufferBase* buffer, ID3D11Device* device) :
    format(buffer->Format())
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = static_cast<UINT>(buffer->GetBufferSize());
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resource = {};
    resource.pSysMem = buffer->GetBufferPtr();

    HRESULT created = device->CreateBuffer(&bd, &resource, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(created), "CreateBuffer");
}

DirectX11ConstantBuffer::DirectX11ConstantBuffer(size_t size, ID3D11Device* device) :
    size(size)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = static_cast<UINT>(size);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT created = device->CreateBuffer(&bd, nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(created), "CreateBuffer");
}

void DirectX11ConstantBuffer::Update(const void* data, size_t size, ID3D11DeviceContext *context)
{
    DXP_ASSERT(size == this->size, "Constant buffer update size mismatch: {} != {}", size, this->size);

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    context->Map(ptr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, data, size);
    context->Unmap(ptr.Get(), 0);
}

};
