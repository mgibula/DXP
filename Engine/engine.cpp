#include "engine.h"
#include "platform.h"

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

    int64_t ticks_per_second, ticks_render_start, ticks_render_end, ticks_last_second = 0;
    ticks_per_second = platform->GetTickFrequency();
    ticks_render_start = ticks_render_end = platform->GetTickCounter();

    while (!IsTerminated()) {
        // Record when frame processing starts
        ticks_render_start = platform->GetTickCounter();
        float32_t delta_time = (float32_t(ticks_render_start) - ticks_render_end) / ticks_per_second;

        OnFrameStart();

        if (IsTerminated())
            break;

        if (!IsPaused())
            Frame();

        OnFrameEnd();

        // Record when frame processing ends
        ticks_render_end = platform->GetTickCounter();

        if (ticks_render_end - ticks_last_second > ticks_per_second)
            ticks_last_second = ticks_render_end;

        // FPS throttling - calculate how many miliseconds we used and sleep accordingly
        int64_t time_delta = ticks_render_end - ticks_render_start;
        float32_t ms_used = float32_t(time_delta) / (ticks_per_second / 1000);

        int32_t time_budget = (desiredFPS) ? 1000 / desiredFPS : 0;
        if (time_budget && time_budget - ms_used > 0)
            platform->SleepMilliseconds(int32_t(time_budget - ms_used));
    }

    PostRenderLoop();
}

void Engine::PreRenderLoop()
{
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGui::StyleColorsDark();

    //renderBackend->PreRenderLoop();
    //platform->PreRenderLoop();
}

void Engine::PostRenderLoop()
{
    //renderBackend->PostRenderLoop();
    //platform->PostRenderLoop();

    //ImGui::DestroyContext();
}

void Engine::OnFrameStart()
{
    //renderBackend->ClearScreen();

    //renderBackend->OnFrameStart();
    platform->OnFrameStart();

    //ImGui::NewFrame();
}

void Engine::OnFrameEnd()
{
    //renderBackend->OnFrameEnd();
    platform->OnFrameEnd();

    //renderBackend->Display();
}

void Engine::Frame()
{

}

};
