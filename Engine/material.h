#pragma once

namespace DXP
{

template <typename T>
struct LoadedShader
{
    LoadedShader(std::shared_ptr<T> shader) : 
        program(shader)
    { };

    bool IsConstantBufferUsed(int slot) const {
        return (program->GetConstantBufferLayout(slot) != nullptr);
    };

    std::shared_ptr<T> program;
};

struct Material
{
    Material(RenderBackend* gpu, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader);

    std::unique_ptr<ConstantBufferContent> CreateConstantBufferData(int slot);

    LoadedShader<VertexShader> vertexShader;
    LoadedShader<PixelShader> pixelShader;

    // textures

    std::unique_ptr<ConstantBufferContent> constantBufferPerMaterial;

    std::vector<const ConstantBufferLayout*> constantBufferLayouts;
    std::vector<std::shared_ptr<ConstantBuffer>> constantBuffers;


private:
    void FillConstantBufferFromShader(RenderBackend* gpu, Shader* shader, int slot);

};

};
