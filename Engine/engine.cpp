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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Ordering imposed by ImGui backends
    platform->PreRenderLoop(this);
    gpu->PreRenderLoop();
}

void Engine::PostRenderLoop()
{
    // Ordering imposed by ImGui backends
    gpu->PostRenderLoop();
    platform->PostRenderLoop(this);

    ImGui::DestroyContext();
}

void Engine::OnFrameStart()
{
    // Ordering imposed by ImGui backends
    gpu->OnFrameStart();
    platform->OnFrameStart(this);
    ImGui::NewFrame();

    gpu->ClearScreen();
}

void Engine::OnFrameEnd()
{    
    // Ordering imposed by ImGui backends
    platform->OnFrameEnd(this);
    gpu->OnFrameEnd();

    gpu->Display();

    // Clear pending events vector
    events.clear();
}

void Engine::Frame()
{
    ImGui::ShowDemoWindow();
}

void Engine::SubmitEvent(const Event& event)
{
    events.push_back(event);
}

};
