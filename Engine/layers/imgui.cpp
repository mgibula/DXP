#include "pch.h"
#include "imgui.h"
#include "../Imgui/imgui.h"

namespace DXP
{

void ImGuiLayer::OnAttach(Engine* engine)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.f;
    style.Colors[ImGuiCol_WindowBg].w = 1.f;

    engine->platform->ImGuiInit();
    engine->gpu->ImGuiInit();

    renderTargetOverride = engine->gpu->CreateRenderTexture(engine->gpu->Width(), engine->gpu->Height());
    engine->renderer->GetScene()->renderTarget = renderTargetOverride;

    debug_material = engine->renderer->CreateMaterial("shaders/color.vs", "shaders/color.ps", {});
}

void ImGuiLayer::OnDetach(Engine* engine)
{
    engine->gpu->ImGuiShutdown();
    engine->platform->ImGuiShutdown();

    ImGui::DestroyContext();
}

void ImGuiLayer::OnFrameStart(Engine* engine)
{
    started = std::chrono::high_resolution_clock::now();

    // ImGui startup
    engine->gpu->ImGuiFrameStart();
    engine->platform->ImGuiFrameStart();
    ImGui::NewFrame();

     //Setup main dockspace (thanks imgui_demo.cpp !)
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);
}

void ImGuiLayer::OnFrameEnd(Engine* engine)
{
    for (auto i = engine->layers.begin(); i != engine->layers.end(); i++)
        i->get()->OnImguiFrame(engine);

    static bool show_app = true;
    ImGui::Begin("Application", &show_app);
    ImGui::Image(renderTargetOverride->GetImGuiImage(), ImVec2(1024, 768));
    ImGui::End();

    engine->gpu->BindRenderTarget(engine->gpu->GetScreenRenderTarget().get());

    engine->platform->ImGuiFrameEnd();
    engine->gpu->ImGuiFrameEnd();

    // Multi viewport handling
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    DrawDebugMesh(engine);

    points.clear();
}

void ImGuiLayer::OnImguiFrame(Engine* engine)
{
    std::chrono::duration<float, std::milli> duration = std::chrono::high_resolution_clock::now() - started;
    processingTimes.Push(duration.count());

    static bool demo_opened = true;
    ImGui::ShowDemoWindow(&demo_opened);

    static bool debug_opened, scene_opened;
    static fmt::memory_buffer buffer;

    if (ImGui::Begin("Logs", &debug_opened))
    {
        {   /* Draw FPS graph */
            float highest_processing_time = *std::max_element(processingTimes.Data(), processingTimes.Data() + processingTimes.Size());
            float last_processing_time = processingTimes.Peek();

            ImGui::PushItemWidth(-1);
            FormatToCString(buffer, "Simulation duration: max: {:.2f}ms, last: {:.2f}", highest_processing_time, last_processing_time);
            ImGui::PlotLines("", processingTimes.GetterFunction(), &processingTimes, processingTimes.Size(), 0, buffer.data(), 0.f, FLT_MAX);
        }

        auto& log_buffer_view = engine->memory_sink->GetLogBufferRef();
        if (!log_buffer_view.empty())
            ImGui::TextUnformatted(&log_buffer_view[0], &log_buffer_view[log_buffer_view.size() - 1]);
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::End();

    if (ImGui::Begin("Scene", &scene_opened))
    {
        DXP::SceneRoot* root = engine->renderer->GetScene();
        if (root)
            root->ImGuiDebug(engine);
    }

    ImGui::End();
}

bool ImGuiLayer::OnEvent(Engine* engine, const Event* event)
{
    if (event->type == Event::Type::ApplicationResized) {
        //engine->gpu->Resize(event->ApplicationSize().first, event->ApplicationSize().second);
    }

    return true;
}

void ImGuiLayer::DrawDebugMesh(Engine* engine)
{
    if (points.empty())
        return;

    auto vertices = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_3>>();
    auto colors = std::make_unique<DXP::Buffer<DXP::BufferFormat::Float32_3>>();

    int count = 1;
    for (const auto& data : points) {
        if (count % 3) {
            vertices->PushElement(data.x);
            vertices->PushElement(data.y);
            vertices->PushElement(data.z);
        } else {
            colors->PushElement(data.x);
            colors->PushElement(data.y);
            colors->PushElement(data.z);

            colors->PushElement(data.x);
            colors->PushElement(data.y);
            colors->PushElement(data.z);
        }

        count++;
    }

    debug_mesh = std::make_shared<DXP::Mesh>();
    debug_mesh->topology = Topology::LineList;
    debug_mesh->SetChannel(DXP::VertexShaderInput::Position0, std::move(vertices));
    debug_mesh->SetChannel(DXP::VertexShaderInput::Color0, std::move(colors));

    if (debug_object)
        debug_object->Remove();

    //if (engine->renderer->GetScene()->mainCamera)
    //    debug_object = engine->renderer->GetScene()->mainCamera->AddChild<RenderObject>(debug_mesh, debug_material);
    debug_object = engine->renderer->GetScene()->AddChild<RenderObject>(debug_mesh, debug_material);
}

void ImGuiLayer::DrawLine(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 color)
{
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(color);
}

};
