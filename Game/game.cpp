#include "pch.h"

namespace Simworld
{

std::shared_ptr<DXP::Mesh> mesh;
std::shared_ptr<DXP::Material> material;
DXP::SceneNode* root;
DXP::RenderObject* r1;

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

    material = engine->renderer->CreateMaterial("shaders/solid.vs", "shaders/solid.ps");

    root = engine->renderer->GetScene();
    r1 = root->AddChild<DXP::RenderObject>(mesh, material);

    //r1->ShaderVariables()->Write("offsetX", .5f);
    //r1->ShaderVariables()->Write("offsetY", -.5f);
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{
    r1->MoveBy(.01f, 0.f, 0.f);
}

};
