#pragma once

namespace DXP
{

// Common helper methods for all shaders
struct DirectX11Shader
{
    virtual ~DirectX11Shader() = default;

protected:
    ID3DBlob* CompileShader(std::string_view path, std::string_view content, const char* target);

    Microsoft::WRL::ComPtr< ID3D11ShaderReflection> reflector;
};

struct DirectX11VertexShader : public VertexShader, public DirectX11Shader
{
    DirectX11VertexShader(std::string_view path, std::string_view content, ID3D11Device* device);

    virtual std::vector<BufferLayout> GetInputLayout() override;

    std::vector<BufferLayout> layout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> ptr;
};

struct DirectX11PixelShader : public PixelShader, public DirectX11Shader
{
    DirectX11PixelShader(std::string_view path, std::string_view content, ID3D11Device* device);

    Microsoft::WRL::ComPtr<ID3D11PixelShader> ptr;
};

};
