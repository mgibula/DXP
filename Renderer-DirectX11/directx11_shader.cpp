#include "pch.h"
#pragma comment(lib, "dxguid.lib")

namespace DXP
{

ID3DBlob* DirectX11Shader::CompileShader(std::string_view path, std::string_view content, const char *target)
{
    int flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

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

DirectX11VertexShader::DirectX11VertexShader(std::string_view path, std::string_view content, ID3D11Device *device)
{
    ID3DBlob* compiled = CompileShader(path, content, "vs_5_0");
    HRESULT success = device->CreateVertexShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateVertexShader");

    compiled->Release();
}

std::vector<BufferLayout> DirectX11VertexShader::GetInputLayout()
{
    if (!layout.empty())
        return layout;

    D3D11_SHADER_DESC desc;
    HRESULT success = reflector->GetDesc(&desc);
    DXP_ASSERT(SUCCEEDED(success), "Reflector::GetDesc");

    for (unsigned int i = 0; i < desc.InputParameters; i++) {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        success = reflector->GetInputParameterDesc(i, &paramDesc);
        DXP_ASSERT(SUCCEEDED(success), "Reflector::GetInputParameterDesc");

        auto& slot = layout.emplace_back();

        switch (paramDesc.ComponentType) {
        case D3D_REGISTER_COMPONENT_UINT32:
            slot.type = BufferLayout::UInt32;
            break;
        case D3D_REGISTER_COMPONENT_SINT32:
            slot.type = BufferLayout::Int32;
            break;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            slot.type = BufferLayout::Float32;
            break;
        default:
            Fatal("Unknown component type: {}", static_cast<int>(paramDesc.ComponentType));
        }

        // Calculate number of components
        auto mask = paramDesc.Mask;
        while (mask) {
            slot.count += mask & 1;
            mask >>= 1;
        }

        slot.semanticName = paramDesc.SemanticName;
        slot.semanticIndex = paramDesc.SemanticIndex;
    }

    return layout;
}

DirectX11PixelShader::DirectX11PixelShader(std::string_view path, std::string_view content, ID3D11Device* device)
{
    ID3DBlob* compiled = CompileShader(path, content, "ps_5_0");
    HRESULT success = device->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreatePixelShader");

    compiled->Release();
}

};
