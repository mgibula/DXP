#pragma once
#include <cstdint>

using float32_t = float;
using float64_t = double;

namespace DXP
{

struct Platform;

struct Engine
{
    Engine(Platform* platform) noexcept;
    ~Engine();

    void Run();

    void SetDesiredFPS(int32_t fps) noexcept;
    int32_t DesiredFPS() const noexcept;

    bool IsTerminated() const noexcept;
    bool IsPaused() const noexcept;

    void Terminate() noexcept;
    void TogglePaused() noexcept;

private:
    void PreRenderLoop();
    void PostRenderLoop();
    void OnFrameStart();
    void OnFrameEnd();
    void Frame();

    Platform* platform;
    int32_t desiredFPS = 60;
    bool terminated = false;
    bool paused = false;
};

inline void Engine::SetDesiredFPS(int32_t fps) noexcept
{
    desiredFPS = fps;
}

inline int32_t Engine::DesiredFPS() const noexcept
{
    return desiredFPS;
}

inline bool Engine::IsTerminated() const noexcept
{
    return terminated;
}

inline bool Engine::IsPaused() const noexcept
{
    return paused;
}

inline void Engine::Terminate() noexcept
{
    terminated = true;
}

inline void Engine::TogglePaused() noexcept
{
    paused = !paused;
}

};
