#include "pch.h"

namespace Simworld
{

std::shared_ptr<DXP::Mesh> mesh;
DXP::SceneNode* root;

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    SPDLOG_LOGGER_INFO(log, "Starting");

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

    root = engine->renderer->GetScene();
    DXP::RenderObject* r1 = root->AddChild<DXP::RenderObject>(mesh);
    r1->position.x = 0.1f;
    r1->mode = DXP::Renderer::Mode::SolidColor;
    r1->GetVariables(DXP::ConstantBufferSlot::Transform)->Set("offsetX", 0.1f);

}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{
}

};
