#pragma once

namespace DXP
{

// Common helper methods for all shaders
struct DirectX11Shader
{
    virtual ~DirectX11Shader() = default;

protected:
    ID3DBlob* CompileShader(std::string_view path, std::string_view content, const char* target, const std::shared_ptr<spdlog::logger>& log);
    void ReflectShader(const std::shared_ptr<spdlog::logger>& log);

    const ConstantBufferLayout* GetConstantBufferLayout(int slot) const {
        if (constantBufferLayouts.size() <= slot)
            return nullptr;

        const ConstantBufferLayout* result = &constantBufferLayouts[slot];
        if (!result->size)
            return nullptr;
        
        return result;
    };

    std::vector<ConstantBufferLayout> constantBufferLayouts;
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;
};

struct DirectX11VertexShader : public VertexShader, public DirectX11Shader
{
    DirectX11VertexShader(std::string_view path, std::string_view content, ID3D11Device* device, std::shared_ptr<spdlog::logger> log);

    std::vector<VertexShaderInput> ReadInputFormat(ID3D11Device* device, ID3DBlob *compiled);

    virtual const std::vector<VertexShaderInput>& GetInputFormat() const override {
        return inputFormat;
    };

    virtual const ConstantBufferLayout* GetConstantBufferLayout(int slot) const override {
        return DirectX11Shader::GetConstantBufferLayout(slot);
    };

    virtual std::string DebugName() const override {
        return path;
    };

    std::string path;
    std::vector<VertexShaderInput> inputFormat;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> ptr;
    Microsoft::WRL::ComPtr<ID3DBlob> bytecode;
};

struct DirectX11PixelShader : public PixelShader, public DirectX11Shader
{
    DirectX11PixelShader(std::string_view path, std::string_view content, ID3D11Device* device, std::shared_ptr<spdlog::logger> log);

    virtual const ConstantBufferLayout* GetConstantBufferLayout(int slot) const override {
        return DirectX11Shader::GetConstantBufferLayout(slot);
    };

    virtual std::string DebugName() const override {
        return path;
    };

    std::string path;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> ptr;
};

};
