#include "pch.h"
#include "imgui_layer.h"
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

    engine->platform->ImGuiInit();
    engine->gpu->ImGuiInit();
}

void ImGuiLayer::OnDetach(Engine* engine)
{
    engine->gpu->ImGuiShutdown();
    engine->platform->ImGuiShutdown();

    ImGui::DestroyContext();
}

void ImGuiLayer::OnFrameStart(Engine* engine)
{
    // ImGui startup
    engine->gpu->ImGuiFrameStart();
    engine->platform->ImGuiFrameStart();
    ImGui::NewFrame();

    // Setup main dockspace (thanks imgui_demo.cpp !)
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void ImGuiLayer::OnFrameEnd(Engine* engine)
{
    for (auto i = engine->layers.begin(); i != engine->layers.end(); i++)
        i->get()->OnImguiFrame(engine);

    engine->platform->ImGuiFrameEnd();
    engine->gpu->ImGuiFrameEnd();

    // Multi viewport handling
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void ImGuiLayer::OnImguiFrame(Engine* engine)
{
    ImGui::ShowDemoWindow();

    static bool logs_opened;
    if (ImGui::Begin("Logs", &logs_opened))
    {
        auto& log_buffer_view = engine->memory_sink->GetLogBufferRef();
        if (!log_buffer_view.empty())
            ImGui::TextUnformatted(&log_buffer_view[0], &log_buffer_view[log_buffer_view.size() - 1]);
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::End();
}

bool ImGuiLayer::OnEvent(Engine* engine, const Event* event)
{
    if (event->type == Event::Type::ApplicationResized) {
        engine->gpu->Resize(event->ApplicationSize().first, event->ApplicationSize().second);
    }

    return true;
}

};
