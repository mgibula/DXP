#pragma once

namespace DXP
{

struct VertexShader;
struct PixelShader;
struct RenderBackend;
struct RendererState;

struct Renderer
{
    friend struct RendererState;

    Renderer(std::shared_ptr<spdlog::logger> log);

    void SetRenderBackend(RenderBackend* backend);
    std::shared_ptr<RendererState> CreateState(std::string name);

    void Draw(std::shared_ptr<RendererState> state, std::shared_ptr<Mesh> mesh);

private:
    void SetCurrentState(const std::shared_ptr<RendererState>& state);

    std::shared_ptr<RendererState> currentState;

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
