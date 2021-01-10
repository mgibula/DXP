#pragma once

using float32_t = float;
using float64_t = double;

namespace DXP
{

struct RenderBackend;
struct Platform;
struct Event;
struct CyclicLog;
struct Simulation;
struct ImGuiLayer;
struct Renderer;

struct FrameInfo
{
    uint64_t frame = 0;
    float deltaTime = 0.f;
};

struct Engine
{
    friend struct ImGuiLayer;

    Engine(Platform* platform, Simulation *simulation) noexcept;
    ~Engine();

    void Run();

    void SetDesiredFPS(int32_t fps) noexcept;
    int32_t DesiredFPS() const noexcept;

    bool IsTerminated() const noexcept;
    bool IsPaused() const noexcept;

    void Terminate() noexcept;
    void TogglePaused() noexcept;

    void SubmitEvent(const Event& event);

    void AddLogSink(std::shared_ptr<spdlog::sinks::sink> sink);
    std::shared_ptr<spdlog::logger> CreateLogger(std::string_view name);

    void PushLayer(std::unique_ptr<Layer> layer);
    std::unique_ptr<Layer> PopLayer();

    std::unique_ptr<Renderer> renderer;

private:
    void PreRenderLoop();
    void PostRenderLoop();
    void OnFrameStart();
    void OnFrameEnd();
    void Frame(const FrameInfo& frame);

    Platform* platform;
    Simulation* simulation;
    std::unique_ptr<RenderBackend> gpu;
    int32_t desiredFPS = 60;
    bool terminated = false;
    bool paused = false;

    std::vector<std::unique_ptr<Layer>> layers;

    std::shared_ptr<CyclicLogSinkST> memory_sink;
    std::vector<std::shared_ptr<spdlog::sinks::sink>> log_sinks;
    std::shared_ptr<spdlog::logger> log;
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
