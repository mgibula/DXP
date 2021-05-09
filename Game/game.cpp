#include "pch.h"

namespace Simworld
{

std::shared_ptr<DXP::Mesh> mesh;
std::shared_ptr<DXP::Material> material;
std::shared_ptr<DXP::Texture> texture;
DXP::SceneRoot* root;
DXP::RenderObject* r1;
DXP::Camera* c1, *c2;
DXP::SceneNode* n1;
//uv - mapper

static void PrepareCube(DXP::Mesh *mesh)
{
    auto vertices = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_3>>();
    auto colors = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_3>>();
    auto triangles = std::make_unique<DXP::Buffer<DXP::BufferFormat::Uint16_1>>();

    // Front wall
    vertices->PushElements({ -1.f, -1.f, -1.f });    // 0 - near bottom left
    vertices->PushElements({ 1.f, -1.f, -1.f });     // 1 - near bottom right
    vertices->PushElements({ 1.f, 1.f, -1.f });      // 2 - near top right
    vertices->PushElements({ -1.f, 1.f, -1.f });     // 3 - near top left

    colors->PushElements({ 0.f, 0.f, 1.f });
    colors->PushElements({ 0.f, 0.f, 1.f });
    colors->PushElements({ 0.f, 0.f, 1.f });
    colors->PushElements({ 0.f, 0.f, 1.f });

    triangles->PushElements({ 0, 3, 2 });
    triangles->PushElements({ 0, 2, 1 });

    // Left wall
    vertices->PushElements({ -1.f, -1.f, -1.f });    // 4 - near bottom left
    vertices->PushElements({ -1.f, 1.f, -1.f });     // 5 - near top left
    vertices->PushElements({ -1.f, -1.f, 1.f });     // 6 - far bottom left
    vertices->PushElements({ -1.f, 1.f, 1.f });      // 7 - far top left

    colors->PushElements({ 0.f, 1.f, 0.f });
    colors->PushElements({ 0.f, 1.f, 0.f });
    colors->PushElements({ 0.f, 1.f, 0.f });
    colors->PushElements({ 0.f, 1.f, 0.f });

    triangles->PushElements({ 6, 7, 5 });
    triangles->PushElements({ 6, 5, 4 });

    // Right wall
    vertices->PushElements({ 1.f, -1.f, -1.f });    // 8 - near bottom right
    vertices->PushElements({ 1.f, 1.f, -1.f });     // 9 - near top right
    vertices->PushElements({ 1.f, -1.f, 1.f });     // 10 - far bottom right
    vertices->PushElements({ 1.f, 1.f, 1.f });      // 11 - far top right

    colors->PushElements({ 1.f, 0.f, 0.f });
    colors->PushElements({ 1.f, 0.f, 0.f });
    colors->PushElements({ 1.f, 0.f, 0.f });
    colors->PushElements({ 1.f, 0.f, 0.f });

    triangles->PushElements({ 8, 9, 11 });
    triangles->PushElements({ 8, 11, 10 });

    // Back wall
    vertices->PushElements({ -1.f, -1.f, 1.f });    // 12 - far bottom left
    vertices->PushElements({ -1.f, 1.f, 1.f });     // 13 - far top left
    vertices->PushElements({ 1.f, -1.f, 1.f });     // 14 - far bottom right
    vertices->PushElements({ 1.f, 1.f, 1.f });      // 15 - far top right

    colors->PushElements({ 1.f, 1.f, 0.f });
    colors->PushElements({ 1.f, 1.f, 0.f });
    colors->PushElements({ 1.f, 1.f, 0.f });
    colors->PushElements({ 1.f, 1.f, 0.f });

    triangles->PushElements({ 12, 15, 13 });
    triangles->PushElements({ 12, 14, 15 });

    // Top wall
    vertices->PushElements({ -1.f, 1.f, -1.f });    // 16 - near top left
    vertices->PushElements({ -1.f, 1.f, 1.f });     // 17 - far top left
    vertices->PushElements({ 1.f, 1.f, -1.f });     // 18 - near top right
    vertices->PushElements({ 1.f, 1.f, 1.f });      // 19 - far top right

    colors->PushElements({ 1.f, 0.f, 1.f });
    colors->PushElements({ 1.f, 0.f, 1.f });
    colors->PushElements({ 1.f, 0.f, 1.f });
    colors->PushElements({ 1.f, 0.f, 1.f });

    triangles->PushElements({ 16, 17, 19 });
    triangles->PushElements({ 16, 19, 18 });

    // Bottom wall
    vertices->PushElements({ -1.f, -1.f, -1.f });    // 20 - near bottom left
    vertices->PushElements({ -1.f, -1.f, 1.f });     // 21 - far bottom left
    vertices->PushElements({ 1.f, -1.f, -1.f });     // 22 - near bottom right
    vertices->PushElements({ 1.f, -1.f, 1.f });      // 23 - far bottom right

    colors->PushElements({ 0.f, 1.f, 1.f });
    colors->PushElements({ 0.f, 1.f, 1.f });
    colors->PushElements({ 0.f, 1.f, 1.f });
    colors->PushElements({ 0.f, 1.f, 1.f });

    triangles->PushElements({ 21, 20, 22 });
    triangles->PushElements({ 21, 22, 23 });

    mesh->SetChannel(DXP::VertexShaderInput::Position0, std::move(vertices));
    mesh->SetChannel(DXP::VertexShaderInput::Color0, std::move(colors));
    mesh->indices = std::move(triangles);
    mesh->rasterizer = DXP::Rasterizer_Wireframe;
}

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    SPDLOG_LOGGER_INFO(log, "Starting");

    mesh = std::make_shared<DXP::Mesh>();
    material = engine->renderer->CreateMaterial("shaders/color.vs", "shaders/color.ps", {});

#if 0

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
#endif

    PrepareCube(mesh.get());

    root = engine->renderer->GetScene();
    r1 = root->AddChild<DXP::RenderObject>(mesh, material);

    c2 = root->AddChild<DXP::Camera>();
    c2->SetName("Camera 2");
    c2->SetPerspective(1.f);
    c2->MoveTo(0.5f, 0.5f, 5.f);
    //c2->drawDebug = true;

    r1->SetName("square 1");
    r1->MoveTo(0.f, 0.f, 10.f);
    r1->RotateTo(0.f, 0.f, 0.f);

    n1 = root->AddChild<DXP::SceneNode>();
    n1->SetName("Node 1");

    c1 = n1->AddChild<DXP::Camera>();
    c1->SetName("Camera 1");
    //c1->MoveTo(0.f, 0.f, -1.5f);

    c1->SetPerspective(1.f);
    //c1->SetOrthographic(2.f);
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

    //engine->debug->DrawLine({ -0.5f, -0.5f, 0.f }, { 0.5f, 0.5f, 1.f }, { 0.f, 1.f, 1.f });

    //r1->RotateBy(0.5f, 0.5f, 0.5f);
     
    //r1->MoveBy(.001f, 0.f, 0.f);
    //c1->MoveBy(.001f, 0.f, 0.f);
    //c1->LookAt(0.f, 0.f, 1.f);
}

};
