#include "pch.h"

namespace DXP
{

Renderer::Renderer(std::shared_ptr<spdlog::logger> log) :
    log(log)
{
    log->info("Initializing");
}

void Renderer::SetRenderBackend(RenderBackend* backend)
{
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
    log->info("Created");
}

void RendererState::LoadVertexShader(std::string_view path)
{
    vertexShader = renderer->gpu->LoadVertexShader(FileGetContent(path));
}

void RendererState::LoadPixelShader(std::string_view path)
{
    pixelShader = renderer->gpu->LoadPixelShader(FileGetContent(path));
}

};
