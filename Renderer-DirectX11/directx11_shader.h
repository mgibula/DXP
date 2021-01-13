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

    void CreateInputLayout(ID3D11Device* device, ID3DBlob *compiled);

    virtual std::vector<BufferLayout> GetInputLayout() override;
    virtual std::string DebugName() const override {
        return path;
    };

    //std::vector<BufferLayout> layout;
    std::string path;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> ptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
};

struct DirectX11PixelShader : public PixelShader, public DirectX11Shader
{
    DirectX11PixelShader(std::string_view path, std::string_view content, ID3D11Device* device);

    virtual std::string DebugName() const override {
        return path;
    };

    std::string path;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> ptr;
};

};
