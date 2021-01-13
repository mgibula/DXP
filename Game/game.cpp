#include "pch.h"

namespace Simworld
{

std::shared_ptr<DXP::RendererState> state;
DXP::Buffer<float32_t, 3> vertices;
DXP::Buffer<int16_t, 3> triangles;
std::shared_ptr<DXP::Mesh> mesh;

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    SPDLOG_LOGGER_INFO(log, "Starting");

    state = engine->renderer->CreateState("render-state-1");
    state->LoadVertexShader("shaders/solid.vs");
    state->LoadPixelShader("shaders/solid.ps");

    vertices.PushElements({ -0.5f, -0.5f, 0.0f });
    vertices.PushElements({ -0.5f, 0.5f, 0.0f });
    vertices.PushElements({ 0.5f, 0.5f, 0.0f });
    vertices.PushElements({ 0.5f, -0.5f, 0.0f });

    triangles.PushElements({ 0, 1, 2 });
    triangles.PushElements({ 0, 2, 3 });

    SPDLOG_LOGGER_INFO(log, "Test float buffer component count: {}, component size: {}, raw size: {}, elements: {}", vertices.ComponentCount(), vertices.ComponentSize(), vertices.GetBufferSize(), vertices.Elements());
    SPDLOG_LOGGER_INFO(log, "Test int16 buffer component count: {}, component size: {}, raw size: {}, elements: {}", triangles.ComponentCount(), triangles.ComponentSize(), triangles.GetBufferSize(), triangles.Elements());

    mesh = std::make_shared<DXP::Mesh>();
    mesh->vertices = vertices;
    mesh->triangles = std::make_unique<decltype(triangles)>(triangles);
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{
    engine->renderer->Draw(state, mesh);
}

};
