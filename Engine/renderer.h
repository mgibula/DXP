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

    Renderer* renderer = nullptr;
    std::shared_ptr<spdlog::logger> log;
};

};
