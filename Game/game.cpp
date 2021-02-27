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

    auto triangles = std::make_unique<DXP::Buffer<DXP::BufferFormat::Uint16_1>>();
    triangles->PushElement(0);
    triangles->PushElement(1);
    triangles->PushElement(2);
    mesh->indices = std::move(triangles);

    auto vertices = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_3>>();
    vertices->PushElement(0.f);
    vertices->PushElement(0.5f);
    vertices->PushElement(0.f);

    vertices->PushElement(0.5f);
    vertices->PushElement(-0.5f);
    vertices->PushElement(0.f);

    vertices->PushElement(-0.5f);
    vertices->PushElement(-0.5f);
    vertices->PushElement(0.f);

    mesh->SetChannel(DXP::VertexShaderInput::Position0, std::move(vertices));
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{
    engine->renderer->Draw(state, mesh);
}

};
