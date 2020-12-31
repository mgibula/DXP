#include "pch.h"
#include "engine.h"
#include "event.h"
#include "platform.h"
#include "render_backend.h"
#include "../Imgui/imgui.h"

namespace DXP
{

Engine::Engine(Platform* platform) noexcept :
    platform(platform)
{

}

Engine::~Engine()
{

}

void Engine::Run()
{
    PreRenderLoop();

    while (!IsTerminated()) {
        // Record when frame processing starts
        int32_t time_budget = (desiredFPS) ? 1000 / desiredFPS : 0;
        auto start = std::chrono::high_resolution_clock::now();

        OnFrameStart();

        if (IsTerminated())
            break;

        if (!IsPaused())
            Frame();

        OnFrameEnd();

        // FPS throttling - calculate how long should we sleep
        if (desiredFPS)
            std::this_thread::sleep_until(start + std::chrono::milliseconds(time_budget));
    }

    PostRenderLoop();
}

void Engine::PreRenderLoop()
{
    gpu = platform->CreateRenderBackend("DirectX11");

    platform->PreRenderLoop(this);
    gpu->PreRenderLoop(this);

    // ImGui setup
    ImGuiInit();
}

void Engine::PostRenderLoop()
{
    // ImGui shutdown
    ImGuiShutdown();

    gpu->PostRenderLoop(this);
    platform->PostRenderLoop(this);
}

void Engine::OnFrameStart()
{
    platform->OnFrameStart(this);
    gpu->OnFrameStart(this);

    // ImGui startup
    ImGuiFrameStart();

    // Setup main dockspace (thanks imgui_demo.cpp !)
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void Engine::OnFrameEnd()
{
    // ImGui finalization
    ImGuiFrameEnd();

    gpu->OnFrameEnd(this);
    platform->OnFrameEnd(this);

    // Display frame
    gpu->Display();

    // Clear pending events vector
    events.clear();

    // Clear now to save time later
    gpu->ClearScreen();
}

void Engine::Frame()
{
    ImGui::ShowDemoWindow();

    ImGui::Begin("Another Window");
    ImGui::Text("Hello from another window!");
    ImGui::End();

}

void Engine::ImGuiInit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    platform->ImGuiInit();
    gpu->ImGuiInit();
}

void Engine::ImGuiShutdown()
{
    gpu->ImGuiShutdown();
    platform->ImGuiShutdown();

    ImGui::DestroyContext();
}

void Engine::ImGuiFrameStart()
{
    gpu->ImGuiFrameStart();
    platform->ImGuiFrameStart();
    ImGui::NewFrame();
}

void Engine::ImGuiFrameEnd()
{
    platform->ImGuiFrameEnd();
    gpu->ImGuiFrameEnd();

    // Multi viewport handling
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void Engine::SubmitEvent(const Event& event)
{
    events.push_back(event);
}

};
