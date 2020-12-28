#include "engine.h"
#include "platform.h"
#include "render_backend.h"
#include "../Imgui/imgui.h"
#include <chrono>
#include <thread>

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

    gpu->PreRenderLoop();
    platform->PreRenderLoop();
}

void Engine::PostRenderLoop()
{
    gpu->PostRenderLoop();
    platform->PostRenderLoop();

    ImGui::DestroyContext();
}

void Engine::OnFrameStart()
{
    gpu->ClearScreen();
    gpu->OnFrameStart();

    platform->OnFrameStart();

    ImGui::NewFrame();
}

void Engine::OnFrameEnd()
{
    ImGui::ShowDemoWindow();

    ImGui::Render();

    gpu->OnFrameEnd();
    platform->OnFrameEnd();

    gpu->Display();
}

void Engine::Frame()
{

}

};
