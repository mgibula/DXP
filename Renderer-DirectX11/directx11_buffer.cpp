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

};
