#include "pch.h"
#pragma comment(lib, "dxguid.lib")

namespace DXP
{

ID3DBlob* DirectX11Shader::CompileShader(std::string_view path, std::string_view content, const char *target)
{
    int flags = D3DCOMPILE_ENABLE_STRICTNESS;
    flags |= D3DCOMPILE_DEBUG;

    ID3DBlob* compiled, * errors;
    HRESULT success = D3DCompile2(
        content.data(),                     /* pSrcData */
        content.length(),                   /* SrcDataSize */
        path.data(),                        /* pSourceName */
        nullptr,                            /* pDefines */
        D3D_COMPILE_STANDARD_FILE_INCLUDE,  /* pInclude */
        "main",                             /* pEntrypoint */
        target,                             /* pTarget */
        flags,                              /* Flags1 */
        0,                                  /* Flags2 */
        0,                                  /* SecondaryDataFlags */
        nullptr,                            /* pSecondaryData */
        0,                                  /* SecondaryDataSize */
        &compiled,                          /* ppCode */
        &errors                             /* ppErrorMsgs */
    );

    if (errors)
        Fatal(FMT_STRING("Shader compilation failed: {}"), static_cast<char *>(errors->GetBufferPointer()));

    success = D3DReflect(compiled->GetBufferPointer(), compiled->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(reflector.GetAddressOf()));
    DXP_ASSERT(SUCCEEDED(success), "D3DReflect");

    return compiled;
}

DirectX11VertexShader::DirectX11VertexShader(std::string_view path, std::string_view content, ID3D11Device *device) :
    path(path)
{
    ID3DBlob* compiled = CompileShader(path, content, "vs_5_0");
    HRESULT success = device->CreateVertexShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateVertexShader");

    CreateInputLayout(device, compiled);

    compiled->Release();
}

void DirectX11VertexShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* compiled)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

    D3D11_SHADER_DESC desc;
    HRESULT success = reflector->GetDesc(&desc);
    DXP_ASSERT(SUCCEEDED(success), "Reflector::GetDesc");

    for (unsigned int i = 0; i < desc.InputParameters; i++) {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        success = reflector->GetInputParameterDesc(i, &paramDesc);
        DXP_ASSERT(SUCCEEDED(success), "Reflector::GetInputParameterDesc");

        auto& slot = layout.emplace_back();
        slot.SemanticName = paramDesc.SemanticName;
        slot.SemanticIndex = paramDesc.SemanticIndex;
        slot.InputSlot = i;
        slot.AlignedByteOffset = 0;
        slot.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        slot.InstanceDataStepRate = 0;

        // Calculate number of components
        auto mask = paramDesc.Mask;
        int components = 0;
        while (mask) {
            components += mask & 1;
            mask >>= 1;
        }

        auto& cacheEntry = layoutCache.emplace_back();
        cacheEntry.count = components;

        switch (paramDesc.ComponentType) {
        case D3D_REGISTER_COMPONENT_UINT32:
            cacheEntry.type = BufferLayout::Type::UInt32;

            if (components == 1) {
                slot.Format = DXGI_FORMAT_R32_UINT;
            } else if (components == 2) {
                slot.Format = DXGI_FORMAT_R32G32_UINT;
            } else if (components == 3) {
                slot.Format = DXGI_FORMAT_R32G32B32_UINT;
            } else if (components == 4) {
                slot.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            } else {
                Fatal("Shader input - uint32 with {} components", components);
            }

            break;
        case D3D_REGISTER_COMPONENT_SINT32:
            cacheEntry.type = BufferLayout::Type::Int32;

            if (components == 1) {
                slot.Format = DXGI_FORMAT_R32_SINT;
            } else if (components == 2) {
                slot.Format = DXGI_FORMAT_R32G32_SINT;
            } else if (components == 3) {
                slot.Format = DXGI_FORMAT_R32G32B32_SINT;
            } else if (components == 4) {
                slot.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            } else {
                Fatal("Shader input - sint32 with {} components", components);
            }

            break;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            cacheEntry.type = BufferLayout::Type::Float32;

            if (components == 1) {
                slot.Format = DXGI_FORMAT_R32_FLOAT;
            } else if (components == 2) {
                slot.Format = DXGI_FORMAT_R32G32_FLOAT;
            } else if (components == 3) {
                slot.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            } else if (components == 4) {
                slot.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            } else {
                Fatal("Shader input - float with {} components", components);
            }

            break;
        default:
            Fatal("Unknown component type: {}/{}", static_cast<int>(paramDesc.ComponentType), components);
        }
    }

    success = device->CreateInputLayout(&layout[0], static_cast<UINT>(layout.size()), compiled->GetBufferPointer(), compiled->GetBufferSize(), this->layout.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateInputLayout");
}

DirectX11PixelShader::DirectX11PixelShader(std::string_view path, std::string_view content, ID3D11Device* device) :
    path(path)
{
    ID3DBlob* compiled = CompileShader(path, content, "ps_5_0");
    HRESULT success = device->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreatePixelShader");

    compiled->Release();
}

};
