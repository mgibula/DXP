#pragma once

namespace DXP
{

struct VertexShader;
struct PixelShader;
struct RenderBackend;
struct RendererState;

struct Renderer
{
    Renderer(std::shared_ptr<spdlog::logger> log);

    void SetRenderBackend(RenderBackend* backend);

    RendererState CreateState(std::string name);

    RenderBackend* gpu;
    std::shared_ptr<spdlog::logger> log;
};

struct RendererState
{
    RendererState(Renderer* renderer, std::shared_ptr<spdlog::logger> log);

    void LoadVertexShader(std::string_view path);
    void LoadPixelShader(std::string_view path);

    std::shared_ptr<VertexShader> vertexShader;
    std::shared_ptr<PixelShader> pixelShader;

    Renderer* renderer;
    std::shared_ptr<spdlog::logger> log;
};

};
