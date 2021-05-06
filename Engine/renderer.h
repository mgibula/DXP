#pragma once

namespace DXP
{

struct VertexShader;
struct PixelShader;
struct RenderBackend;
struct SceneRoot;
struct SceneNode;
struct RenderObject;

struct Renderer
{
    friend struct RendererState;

    Renderer(std::shared_ptr<spdlog::logger> log);

    void SetRenderBackend(RenderBackend* backend);

    std::shared_ptr<Material> CreateMaterial(std::string_view vertexShaderPath, std::string_view pixelShaderPath, std::vector<std::shared_ptr<Texture>> textures);

    std::shared_ptr<Texture> LoadTexture(const TextureData& textureData);

    SceneRoot* GetScene() {
        return scene.get();
    };

    void Draw(Material* material, Mesh *mesh);

    void DrawScene(SceneRoot* root);
    
    std::shared_ptr<VertexShader> LoadVertexShader(std::string_view path);
    std::shared_ptr<PixelShader> LoadPixelShader(std::string_view path);

    // This probably shouldnt be here
    int Width() {
        return gpu->Width();
    };

    int Height() {
        return gpu->Height();
    };

private:
    struct {
        std::unordered_map<std::string, std::shared_ptr<VertexShader>> vertexShaders;
        std::unordered_map<std::string, std::shared_ptr<PixelShader>> pixelShaders;
    } cache;

    std::unique_ptr<SceneRoot> scene;

    // Runtime resources - constant buffers
    std::shared_ptr<ConstantBuffer> transformConstantBuffer;
    std::shared_ptr<ConstantBuffer> cameraConstantBuffer;

    // Predefined samplers
    std::shared_ptr<Sampler> pointSampler;
    std::shared_ptr<Sampler> bilinearSampler;
    std::shared_ptr<Sampler> trilinearSampler;
    std::shared_ptr<Sampler> anisotropicSampler;

    RenderBackend* gpu = nullptr;
    std::shared_ptr<spdlog::logger> log;

    void BindMaterial(DXP::Material* material);
    void UpdateConstantBuffers(DXP::RenderObject* object, DirectX::FXMMATRIX parent);
    void DrawScene(SceneNode* root, DirectX::FXMMATRIX parent);
};

};
