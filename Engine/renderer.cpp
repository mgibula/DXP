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

std::shared_ptr<RendererState> Renderer::CreateState(std::string name)
{
    return std::make_shared<RendererState>(this, log->clone(std::move(name)));
}

void Renderer::Draw(std::shared_ptr<RendererState> state, std::shared_ptr<Mesh> mesh)
{
    SetCurrentState(state);

    if (!mesh->vertexBuffer)
        mesh->vertexBuffer = gpu->LoadVertexBuffer(&mesh->vertices);

    if (mesh->triangles && !mesh->indexBuffer)
        mesh->indexBuffer = gpu->LoadIndexBuffer(mesh->triangles.get());

    std::vector<const VertexBuffer*> buffers;
    buffers.push_back(mesh->vertexBuffer.get());

    gpu->BindVertexBuffers(buffers.data(), buffers.size(), 0);
    
    if (mesh->indexBuffer) {
        gpu->BindIndexBuffer(mesh->indexBuffer.get());
        gpu->DrawIndexed(mesh->triangles->Elements() * mesh->triangles->ComponentCount());
    } else {
        gpu->Draw(mesh->vertices.Elements());
    }
}

void Renderer::SetCurrentState(const std::shared_ptr<RendererState>& state)
{
    if (currentState != state) {
        gpu->BindVertexShader(state->vertexShader.get());
        gpu->BindPixelShader(state->pixelShader.get());
        gpu->BindTopology(state->topology);
        currentState = state;
    }
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
