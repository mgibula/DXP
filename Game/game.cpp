#include "pch.h"

namespace Simworld
{

std::shared_ptr<DXP::Mesh> mesh;
std::shared_ptr<DXP::Material> material;
std::shared_ptr<DXP::Texture> texture;
DXP::SceneRoot* root;
DXP::RenderObject* r1;
DXP::Camera* c1;
DXP::SceneNode* n1;
//uv - mapper

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    SPDLOG_LOGGER_INFO(log, "Starting");

    mesh = std::make_shared<DXP::Mesh>();

    auto triangles = std::make_unique<DXP::Buffer<DXP::BufferFormat::Uint16_1>>();
    triangles->PushElement(0);
    triangles->PushElement(1);
    triangles->PushElement(2);

    triangles->PushElement(0);
    triangles->PushElement(3);
    triangles->PushElement(1);
    mesh->indices = std::move(triangles);

    auto vertices = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_3>>();
    auto uv = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_2>>();

    // top left
    vertices->PushElement(-0.5f);
    vertices->PushElement(0.5f);
    vertices->PushElement(0.f);

    uv->PushElement(0.f);
    uv->PushElement(0.f);

    // bottom right
    vertices->PushElement(0.5f);
    vertices->PushElement(-0.5f);
    vertices->PushElement(0.f);

    uv->PushElement(1.f);
    uv->PushElement(1.f);

    // bottom left
    vertices->PushElement(-0.5f);
    vertices->PushElement(-0.5f);
    vertices->PushElement(0.f);

    uv->PushElement(0.f);
    uv->PushElement(1.f);

    // top right
    vertices->PushElement(0.5f);
    vertices->PushElement(0.5f);
    vertices->PushElement(0.f);

    uv->PushElement(1.f);
    uv->PushElement(0.f);

    mesh->SetChannel(DXP::VertexShaderInput::Position0, std::move(vertices));
    mesh->SetChannel(DXP::VertexShaderInput::UV0, std::move(uv));

    auto textureData = engine->textureLoader.LoadTextureFromFile("textures/uv-mapper.jpg", 4);
    texture = engine->renderer->LoadTexture(textureData);

    material = engine->renderer->CreateMaterial("shaders/solid.vs", "shaders/solid.ps", { texture });

    root = engine->renderer->GetScene();
    r1 = root->AddChild<DXP::RenderObject>(mesh, material);
    r1->SetName("square 1");
    r1->MoveTo(0.f, 0.f, 10.f);
    r1->RotateTo(0.f, 0.f, 0.f);

    n1 = root->AddChild<DXP::SceneNode>();
    n1->SetName("Node 1");

    c1 = n1->AddChild<DXP::Camera>();
    c1->SetName("Camera 1");
    //c1->MoveTo(0.f, 0.f, -1.5f);

    c1->SetPerspective(1.f);
    c1->SetOrthographic(2.f);
    //c1->LookAt(0.f, 0.5f, 1.f);

    root->SetMainCamera(c1);

    c1->MoveTo(0.f, 0.f, 0.f);

    //n1->MoveTo(0.f, 0.f, -1.5f);


    //r1->ShaderVariables()->Write("offsetX", .5f);
    //r1->ShaderVariables()->Write("offsetY", -.5f);
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{
    auto oldPosition = engine->input->GetMousePosition();

    //r1->RotateBy(0.5f, 0.5f, 0.5f);
     
    //r1->MoveBy(.001f, 0.f, 0.f);
    //c1->MoveBy(.001f, 0.f, 0.f);
    //c1->LookAt(0.f, 0.f, 1.f);
}

};
