#pragma once

namespace DXP
{

struct VertexShader;
struct PixelShader;
struct RenderBackend;
struct SceneRoot;
struct SceneNode;
struct RenderObject;
struct Platform;
struct DepthStencilTest;
struct DepthStencilTexture;
struct Viewport;

struct RendererOutput
{
    void Resize(RenderBackend* gpu, int width, int height);

    std::shared_ptr<RenderTarget> renderTarget;
    std::shared_ptr<DepthStencilTest> depthStencilTest;
    std::shared_ptr<DepthStencilTexture> depthStencilTexture;
    std::shared_ptr<Viewport> viewport;
};

struct Resources
{
    // These are used as register numbers in shader as well
    enum {
        Sampler_Point = 0,
        Sampler_Bilinear = 1,
        Sampler_Trilinear = 2,
        Sampler_Anisotropic = 3,
    };
};

struct Renderer
{
    friend struct RendererState;

    Renderer(Platform *platform, RenderBackend* gpu, std::shared_ptr<spdlog::logger> log);

    std::shared_ptr<Material> CreateMaterial(std::string_view vertexShaderPath, std::string_view pixelShaderPath, std::vector<std::shared_ptr<Texture>> textures);

    std::shared_ptr<Texture> LoadTexture(const TextureData& textureData);

    SceneRoot* GetScene() {
        return scene.get();
    };

    void Draw(Material* material, Mesh *mesh);

    void DrawScene(SceneRoot* root);

    void OnScreenResize(int width, int height);

    std::shared_ptr<VertexShader> LoadVertexShader(std::string_view path);
    std::shared_ptr<PixelShader> LoadPixelShader(std::string_view path);

    RendererResources resources;
private:
    struct {
        std::unordered_map<std::string, std::shared_ptr<VertexShader>> vertexShaders;
        std::unordered_map<std::string, std::shared_ptr<PixelShader>> pixelShaders;
    } cache;

    std::unique_ptr<SceneRoot> scene;
    std::shared_ptr<RendererOutput> screenOutput;

    // Runtime resources - constant buffers
    std::shared_ptr<ConstantBuffer> transformConstantBuffer;
    std::shared_ptr<ConstantBuffer> cameraConstantBuffer;

    // Predefined viewports
    std::shared_ptr<Viewport> fullViewport;

    RenderBackend* gpu = nullptr;
    Platform* platform = nullptr;
    std::shared_ptr<spdlog::logger> log;

    void BindMaterial(DXP::Material* material);
    void UpdateConstantBuffers(DXP::RenderObject* object, DirectX::FXMMATRIX parent);
    void DrawScene(SceneNode* root, DirectX::FXMMATRIX parent);

    void SetRenderBackend(RenderBackend* backend);
    void InitCodex();
};

};
