#pragma once

namespace DXP
{

struct VertexShader;
struct PixelShader;
struct RenderBackend;
struct RendererState;
struct SceneNode;

struct Renderer
{
    friend struct RendererState;

    enum Mode {
        None,           // Null shader
        SolidColor,     // Solid color shader
        _size,          // Always last
    };

    Renderer(std::shared_ptr<spdlog::logger> log);

    void SetRenderBackend(RenderBackend* backend);
    std::shared_ptr<RendererState> CreateState(std::string name);

    SceneNode* GetScene() {
        return scene.get();
    };

    void Draw(std::shared_ptr<RendererState> state, Mesh *mesh);

    void DrawScene(SceneNode* root);

private:
    void SetCurrentState(const std::shared_ptr<RendererState>& state);

    std::vector<std::shared_ptr<RendererState>> predefinedStates;
    std::shared_ptr<RendererState> currentState;
    std::unique_ptr<SceneNode> scene;

    RenderBackend* gpu = nullptr;
    std::shared_ptr<spdlog::logger> log;
};

struct RendererState
{
    RendererState() = default;
    RendererState(Renderer* renderer, std::shared_ptr<spdlog::logger> log);

    bool IsValid() const {
        return (renderer && vertexShader && pixelShader);
    };

    void LoadVertexShader(std::string_view path);
    void LoadPixelShader(std::string_view path);

    std::shared_ptr<VertexShader> vertexShader;
    std::shared_ptr<PixelShader> pixelShader;
    Topology topology = Topology::Triangles;

    Renderer* renderer = nullptr;
    std::shared_ptr<spdlog::logger> log;
};

};
