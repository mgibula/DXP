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
    bytecode = CompileShader(path, content, "vs_5_0");
    HRESULT success = device->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreateVertexShader");

    inputFormat = ReadInputFormat(device, bytecode.Get());
}

std::vector<VertexShaderInput> DirectX11VertexShader::ReadInputFormat(ID3D11Device* device, ID3DBlob* compiled)
{
    std::vector<VertexShaderInput> result;

    D3D11_SHADER_DESC desc;
    HRESULT success = reflector->GetDesc(&desc);
    DXP_ASSERT(SUCCEEDED(success), "Reflector::GetDesc");

    for (unsigned int i = 0; i < desc.InputParameters; i++) {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        success = reflector->GetInputParameterDesc(i, &paramDesc);
        DXP_ASSERT(SUCCEEDED(success), "Reflector::GetInputParameterDesc");
        std::string name = paramDesc.SemanticName;

        if (name == "POSITION") {
            switch (paramDesc.SemanticIndex) {
            case 0:
                result.push_back(VertexShaderInput::Position0);
                break;
            case 1:
                result.push_back(VertexShaderInput::Position1);
                break;
            case 2:
                result.push_back(VertexShaderInput::Position2);
                break;
            case 3:
                result.push_back(VertexShaderInput::Position3);
                break;
            default:
                DXP::Fatal("Unsupported semantic index {}/{}", paramDesc.SemanticName, paramDesc.SemanticIndex);
            }
        } else if (name == "COLOR") {
            switch (paramDesc.SemanticIndex) {
            case 0:
                result.push_back(VertexShaderInput::Color0);
                break;
            case 1:
                result.push_back(VertexShaderInput::Color1);
                break;
            case 2:
                result.push_back(VertexShaderInput::Color2);
                break;
            case 3:
                result.push_back(VertexShaderInput::Color3);
                break;
            default:
                DXP::Fatal("Unsupported semantic index {}/{}", paramDesc.SemanticName, paramDesc.SemanticIndex);
            }
        } else if (name == "TEXCOORD") {
            switch (paramDesc.SemanticIndex) {
            case 0:
                result.push_back(VertexShaderInput::UV0);
                break;
            case 1:
                result.push_back(VertexShaderInput::UV1);
                break;
            case 2:
                result.push_back(VertexShaderInput::UV2);
                break;
            case 3:
                result.push_back(VertexShaderInput::UV3);
                break;
            default:
                DXP::Fatal("Unsupported semantic index {}/{}", paramDesc.SemanticName, paramDesc.SemanticIndex);
            }
        } else {
            DXP::Fatal("Unsupported semantic {}/{}", paramDesc.SemanticName, paramDesc.SemanticIndex);
        }
    }

    return result;
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
