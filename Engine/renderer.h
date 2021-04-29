#pragma once

namespace DXP
{

struct VertexShader;
struct PixelShader;
struct RenderBackend;
struct SceneNode;
struct RenderObject;

struct Renderer
{
    friend struct RendererState;

    Renderer(std::shared_ptr<spdlog::logger> log);

    void SetRenderBackend(RenderBackend* backend);

    std::shared_ptr<Material> CreateMaterial(std::string_view vertexShaderPath, std::string_view pixelShaderPath);

    SceneNode* GetScene() {
        return scene.get();
    };

    void Draw(Material* material, Mesh *mesh);

    void DrawScene(SceneNode* root);
    
    std::shared_ptr<VertexShader> LoadVertexShader(std::string_view path);
    std::shared_ptr<PixelShader> LoadPixelShader(std::string_view path);

private:
    struct {
        std::unordered_map<std::string, std::shared_ptr<VertexShader>> vertexShaders;
        std::unordered_map<std::string, std::shared_ptr<PixelShader>> pixelShaders;
    } cache;

    std::unique_ptr<SceneNode> scene;
    std::shared_ptr<ConstantBuffer> transformConstantBuffer;

    RenderBackend* gpu = nullptr;
    std::shared_ptr<spdlog::logger> log;

    void BindConstantBuffers(DXP::RenderObject* object, DirectX::FXMMATRIX parent);
    void DrawScene(SceneNode* root, DirectX::FXMMATRIX parent);
};

};
