#include "pch.h"
#include "cyclic_buffer.h"
#include "cyclic_log.h"
#include "engine.h"
#include "event.h"
#include "platform.h"
#include "render_backend.h"
#include "../Imgui/imgui.h"

#include "spdlog/stopwatch.h"

namespace DXP
{

Engine::Engine(Platform* platform, Simulation* simulation) noexcept :
    platform(platform),
    simulation(simulation)
{
    // Initialize platform (it can inject its log sinks)
    platform->Initialize(this);

    // Add universal in-memory ring buffer sink
    memory_sink = std::make_shared<CyclicLogSinkST>(1024 * 100);
    memory_sink->set_level(spdlog::level::debug);
    log_sinks.push_back(memory_sink);

    // Create core logger and use it as default logger
    log = CreateLogger("core");
    spdlog::set_default_logger(log);
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
    SPDLOG_LOGGER_INFO(log, "Created renderer: {}", gpu->InfoString());

    platform->PreRenderLoop(this);
    gpu->PreRenderLoop(this);
    simulation->PreRenderLoop(this);

    // ImGui setup
    ImGuiInit();
}

void Engine::PostRenderLoop()
{
    // ImGui shutdown
    ImGuiShutdown();

    simulation->PostRenderLoop(this);
    gpu->PostRenderLoop(this);
    platform->PostRenderLoop(this);
}

void Engine::OnFrameStart()
{
    SPDLOG_LOGGER_DEBUG(log, "Frame start");

    platform->OnFrameStart(this);
    gpu->OnFrameStart(this);

    // ImGui startup
    ImGuiFrameStart();

    // Setup main dockspace (thanks imgui_demo.cpp !)
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void Engine::OnFrameEnd()
{
    SPDLOG_LOGGER_DEBUG(log, "Frame end");

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

void Engine::PushLayer(std::unique_ptr<Layer> layer)
{
    layers.push_back(std::move(layer));
    layers.back()->OnAttach(this);
}

std::unique_ptr<Layer> Engine::PopLayer()
{
    if (layers.empty())
        return nullptr;

    std::unique_ptr<Layer> result = std::move(layers.back());
    layers.pop_back();
    
    result->OnDetach(this);

    return result;
}

void Engine::Frame()
{
    ImGuiFrame();

}

void Engine::ImGuiFrame()
{
    static std::string all_messages;
    all_messages.reserve(1024 * 100);
    all_messages.clear();

    ImGui::ShowDemoWindow();
    
    static bool logs_opened;
    if (ImGui::Begin("Logs", &logs_opened))
    {
        auto& log_buffer_view = memory_sink->GetLogBufferRef();
        if (!log_buffer_view.empty())
            ImGui::TextUnformatted(&log_buffer_view[0], &log_buffer_view[log_buffer_view.size() - 1]);
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

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
    switch (event.type) {
    case Event::Type::MouseMoved:
        SPDLOG_LOGGER_TRACE(log, "Event: {}", event.DebugDescription());
        break;
    default:
        SPDLOG_LOGGER_INFO(log, "Event: {}", event.DebugDescription());
        break;
    }

    events.push_back(event);
}

void Engine::AddLogSink(std::shared_ptr<spdlog::sinks::sink> sink)
{
    log_sinks.push_back(sink);
}

std::shared_ptr<spdlog::logger> Engine::CreateLogger(std::string_view name)
{
    auto result = std::make_shared<spdlog::logger>(std::string(name), log_sinks.begin(), log_sinks.end());
    spdlog::register_logger(result);

    return result;
}

};
