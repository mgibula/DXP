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
    Material(RenderBackend* gpu, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, std::vector<std::shared_ptr<Texture>> textures);

    std::unique_ptr<ConstantBufferContent> CreateConstantBufferData(int slot);

    // shaders
    LoadedShader<VertexShader> vertexShader;
    LoadedShader<PixelShader> pixelShader;

    // constant buffers used by shaders
    // there is assumption that all shaders share same format for given CB slot
    std::vector<const ConstantBufferLayout*> constantBufferLayouts;
    std::vector<std::shared_ptr<ConstantBuffer>> constantBuffers;

    // CB data per material
    std::unique_ptr<ConstantBufferContent> constantBufferPerMaterial;

    // textures
    std::vector<std::shared_ptr<Texture>> textures;

private:
    void FillConstantBufferFromShader(RenderBackend* gpu, Shader* shader, int slot);

};

};
