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

static int FindMatchingChannel(BufferBase** buffers, int count, DXP::BufferLayout::Type componentType, int componentCount)
{
    for (int i = 0; i < count; i++) {
        if (buffers[i]->ComponentType() == componentType && buffers[i]->ComponentCount() == componentCount)
            return i;
    }

    return -1;
}

void Renderer::Draw(std::shared_ptr<RendererState> state, std::shared_ptr<Mesh> mesh)
{
    SetCurrentState(state);

    if (mesh->indices && !mesh->indexBuffer)
        mesh->indexBuffer = gpu->LoadIndexBuffer(mesh->indices.get());

    std::array<BufferBase*, 16> buffers;
    int buffersCount = static_cast<int>(mesh->channels.size());
    mesh->vertexBuffers.resize(buffersCount);

    for (int i = 0; i < buffersCount; i++) {
        buffers[i] = mesh->channels[i].get();
    }

    const auto& inputLayout = state->vertexShader->GetInputLayout();
    std::array<const VertexBuffer*, 16> vertexBuffers;
    int vertexBuffersCount = static_cast<int>(inputLayout.size());

    for (int i = 0; i < vertexBuffersCount; i++) {
        const DXP::BufferLayout& layout = inputLayout[i];

        int channel = FindMatchingChannel(&buffers[0], buffersCount, layout.type, layout.count);
        DXP_ASSERT(channel != -1, "Unable to find matching channel in mesh");

        if (!mesh->vertexBuffers[channel])
            mesh->vertexBuffers[channel] = gpu->LoadVertexBuffer(buffers[channel]);

        vertexBuffers[i] = mesh->vertexBuffers[channel].get();
    }

    gpu->BindVertexBuffers(&vertexBuffers[0], vertexBuffersCount, 0);

    if (mesh->indexBuffer) {
        gpu->BindIndexBuffer(mesh->indexBuffer.get());
        gpu->DrawIndexed(mesh->indices->Elements() * mesh->indices->ComponentCount());
    } else {
        gpu->Draw(mesh->channels[0]->Elements());
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
