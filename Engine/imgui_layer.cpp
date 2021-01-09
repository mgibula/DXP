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
    started = std::chrono::high_resolution_clock::now();

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
    std::chrono::duration<float, std::milli> duration = std::chrono::high_resolution_clock::now() - started;
    processingTimes.Push(duration.count());

    ImGui::ShowDemoWindow();

    static bool debug_opened;
    static fmt::memory_buffer buffer;

    if (ImGui::Begin("Debug", &debug_opened))
    {
        {   /* Draw FPS graph */
            float highest_processing_time = *std::max_element(processingTimes.Data(), processingTimes.Data() + processingTimes.Size());
            float last_processing_time = processingTimes.Peek();

            ImGui::PushItemWidth(-1);
            FormatToCString(buffer, "Simulation duration: max: {:.2f}ms, last: {:.2f}", highest_processing_time, last_processing_time);
            ImGui::PlotLines("", processingTimes.GetterFunction(), &processingTimes, processingTimes.Size(), 0, buffer.data(), 0.f, FLT_MAX);
        }

        if (ImGui::BeginTabBar("ImGuiLayer")) {
            if (ImGui::BeginTabItem("Logs")) {
                auto& log_buffer_view = engine->memory_sink->GetLogBufferRef();
                if (!log_buffer_view.empty())
                    ImGui::TextUnformatted(&log_buffer_view[0], &log_buffer_view[log_buffer_view.size() - 1]);

                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
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
