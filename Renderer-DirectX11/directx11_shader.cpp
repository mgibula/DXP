#include "pch.h"
#pragma comment(lib, "dxguid.lib")

namespace DXP
{

ID3DBlob* DirectX11Shader::CompileShader(std::string_view path, std::string_view content, const char *target, const std::shared_ptr<spdlog::logger>& log)
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

    SPDLOG_LOGGER_INFO(log, "Compiled shader {} (target: {}). Bytecode size: {}", path, target, compiled->GetBufferSize());
    ReflectShader(log);
     
    return compiled;
}

void DirectX11Shader::ReflectShader(const std::shared_ptr<spdlog::logger>& log)
{
    D3D11_SHADER_DESC shaderDesc;
    HRESULT success = reflector->GetDesc(&shaderDesc);
    DXP_ASSERT(SUCCEEDED(success), "ID3D11ShaderReflection::GetDesc");

    for (UINT i = 0; i < shaderDesc.ConstantBuffers; i++) {
        ID3D11ShaderReflectionConstantBuffer* cbReflector = reflector->GetConstantBufferByIndex(i);

        D3D11_SHADER_BUFFER_DESC cbDesc;
        success = cbReflector->GetDesc(&cbDesc);
        DXP_ASSERT(SUCCEEDED(success), "ID3D11ShaderReflectionConstantBuffer::GetDesc");

        D3D11_SHADER_INPUT_BIND_DESC bindDesc;
        success = reflector->GetResourceBindingDescByName(cbDesc.Name, &bindDesc);
        DXP_ASSERT(SUCCEEDED(success), "ID3D11ShaderReflection::GetResourceBindingDescByName");

        SPDLOG_LOGGER_INFO(log, "Constant buffer {} (slot {}) size is {} bytes", bindDesc.Name, bindDesc.BindPoint, cbDesc.Size);

        ConstantBufferLayout layout;
        layout.size = cbDesc.Size;

        for (UINT j = 0; j < cbDesc.Variables; j++) {
            ID3D11ShaderReflectionVariable* varReflector = cbReflector->GetVariableByIndex(j);
            D3D11_SHADER_VARIABLE_DESC varDesc;
            success = varReflector->GetDesc(&varDesc);
            DXP_ASSERT(SUCCEEDED(success), "ID3D11ShaderReflectionVariable::GetDesc");

            ID3D11ShaderReflectionType* varTypeReflector = varReflector->GetType();
            D3D11_SHADER_TYPE_DESC varType;
            success = varTypeReflector->GetDesc(&varType);
            DXP_ASSERT(SUCCEEDED(success), "ID3D11ShaderReflectionType::GetDesc");

            SPDLOG_LOGGER_INFO(log, "  - {}", varDesc.Name);

            ConstantBufferField& field = layout.fields.emplace_back();
            field.name = varDesc.Name;
            field.size = varDesc.Size;
            field.offset = varDesc.StartOffset;
        }

        constantBufferLayouts.resize(bindDesc.BindPoint + 1);
        constantBufferLayouts[bindDesc.BindPoint] = layout;
    }
}

DirectX11VertexShader::DirectX11VertexShader(std::string_view path, std::string_view content, ID3D11Device *device, std::shared_ptr<spdlog::logger> log) :
    path(path)
{
    bytecode = CompileShader(path, content, "vs_5_0", log);
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

DirectX11PixelShader::DirectX11PixelShader(std::string_view path, std::string_view content, ID3D11Device* device, std::shared_ptr<spdlog::logger> log) :
    path(path)
{
    ID3DBlob* compiled = CompileShader(path, content, "ps_5_0", log);
    HRESULT success = device->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, ptr.GetAddressOf());
    DXP_ASSERT(SUCCEEDED(success), "CreatePixelShader");

    compiled->Release();
}

};
