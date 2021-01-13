#include "pch.h"

namespace DXP
{

Renderer::Renderer(std::shared_ptr<spdlog::logger> log) :
    log(log)
{
    SPDLOG_LOGGER_INFO(log, "Initializing");
}

void Renderer::SetRenderBackend(RenderBackend* backend)
{
    SPDLOG_LOGGER_INFO(log, "Using renderer backend: {}", backend->InfoString());
    gpu = backend;
}

RendererState Renderer::CreateState(std::string name)
{
    return RendererState(this, log->clone(std::move(name)));
}

RendererState::RendererState(Renderer* renderer, std::shared_ptr<spdlog::logger> log) :
    renderer(renderer),
    log(log)
{
    SPDLOG_LOGGER_INFO(log, "Created");
}

void RendererState::LoadVertexShader(std::string_view path)
{
    vertexShader = renderer->gpu->LoadVertexShader(path, FileGetContent(path));
    SPDLOG_LOGGER_INFO(log, "Loaded vertex shader {}", path);
}

void RendererState::LoadPixelShader(std::string_view path)
{
    pixelShader = renderer->gpu->LoadPixelShader(path, FileGetContent(path));
    SPDLOG_LOGGER_INFO(log, "Loaded pixel shader {}", path);
}

};
