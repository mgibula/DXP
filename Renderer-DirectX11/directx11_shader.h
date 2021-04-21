#pragma once

namespace DXP
{

// Common helper methods for all shaders
struct DirectX11Shader
{
    virtual ~DirectX11Shader() = default;

protected:
    ID3DBlob* CompileShader(std::string_view path, std::string_view content, const char* target, const std::shared_ptr<spdlog::logger>& log);
    ConstantBufferLayout ReadConstantBufferLayout(int slot, const std::shared_ptr<spdlog::logger>& log);

    void InitializeConstantBuffer(ConstantBufferSlot slot, const std::shared_ptr<spdlog::logger>& log);

    std::vector<ConstantBufferLayout> constantBufferLayouts;
    Microsoft::WRL::ComPtr< ID3D11ShaderReflection> reflector;
};

struct DirectX11VertexShader : public VertexShader, public DirectX11Shader
{
    DirectX11VertexShader(std::string_view path, std::string_view content, ID3D11Device* device, std::shared_ptr<spdlog::logger> log);

    std::vector<VertexShaderInput> ReadInputFormat(ID3D11Device* device, ID3DBlob *compiled);

    virtual const std::vector<VertexShaderInput>& GetInputFormat() const override {
        return inputFormat;
    };

    virtual const ConstantBufferLayout* GetConstantBufferLayout(ConstantBufferSlot slot) const override {
        return &constantBufferLayouts[static_cast<int>(slot)];
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

    virtual const ConstantBufferLayout* GetConstantBufferLayout(ConstantBufferSlot slot) const override {
        return &constantBufferLayouts[static_cast<int>(slot)];
    };

    virtual std::string DebugName() const override {
        return path;
    };

    std::string path;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> ptr;
};

};
