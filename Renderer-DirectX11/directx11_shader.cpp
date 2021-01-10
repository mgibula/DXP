#include "pch.h"

namespace DXP
{

ID3DBlob* DirectX11Shader::CompileShader(std::string_view content, const char *target)
{
    int flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* compiled, * errors;
    HRESULT success = D3DCompile2(
        content.data(),                     /* pSrcData */
        content.length(),                   /* SrcDataSize */
        nullptr,                            /* pSourceName */
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
        Fatal(FMT_STRING("Shader compilation failed: {}"), errors->GetBufferPointer());

    return compiled;
}

DirectX11VertexShader::DirectX11VertexShader(std::string_view content, ID3D11Device *device)
{
    ID3DBlob* compiled = CompileShader(content, "vs_5");
    HRESULT success = device->CreateVertexShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    compiled->Release();

    DXP_ASSERT(SUCCEEDED(success), "CreateVertexShader");
}

DirectX11PixelShader::DirectX11PixelShader(std::string_view content, ID3D11Device* device)
{
    ID3DBlob* compiled = CompileShader(content, "ps_5");
    HRESULT success = device->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    compiled->Release();

    DXP_ASSERT(SUCCEEDED(success), "CreatePixelShader");
}

};
