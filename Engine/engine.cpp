#include "pch.h"
#include "engine.h"
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
    gpu->PreRenderLoop();
    platform->PreRenderLoop();
}

void Engine::PostRenderLoop()
{
    platform->PostRenderLoop();
    gpu->PostRenderLoop();
}

void Engine::OnFrameStart()
{
    gpu->OnFrameStart();
    platform->OnFrameStart();

    gpu->ClearScreen();
}

void Engine::OnFrameEnd()
{
    ImGui::ShowDemoWindow();
    
    platform->OnFrameEnd();
    gpu->OnFrameEnd();

    gpu->Display();
}

void Engine::Frame()
{

}

};
