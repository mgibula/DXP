#include "pch.h"

namespace Simworld
{

std::shared_ptr<DXP::RendererState> state;
std::shared_ptr<DXP::Mesh> mesh;

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    SPDLOG_LOGGER_INFO(log, "Starting");

    state = engine->renderer->CreateState("render-state-1");
    state->LoadVertexShader("shaders/solid.vs");
    state->LoadPixelShader("shaders/solid.ps");

    mesh = std::make_shared<DXP::Mesh>();

    auto triangles = std::make_unique<DXP::Buffer<int16_t, 3>>();
    triangles->PushElements({ 0, 1, 2 });
    mesh->indices = std::move(triangles);

    auto vertices = std::make_unique<DXP::Buffer<float32_t, 3>>();
    vertices->PushElements({ 0.f, 0.5f, 0.0f });        // bottom left
    vertices->PushElements({ 0.5f, -0.5f, 0.0f });      // top left
    vertices->PushElements({ -0.5f, -0.5f, 0.0f });     // top right
    mesh->channels.push_back(std::move(vertices));
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{
    engine->renderer->Draw(state, mesh);
}

};
