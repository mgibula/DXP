#include "pch.h"
#include "cyclic_buffer.h"
#include "cyclic_log.h"
#include "engine.h"
#include "event.h"
#include "platform.h"
#include "render_backend.h"

namespace DXP
{

Engine::Engine(Platform* platform, Simulation* simulation) noexcept :
    platform(platform),
    simulation(simulation)
{
    // Add universal in-memory ring buffer sink
    memory_sink = std::make_shared<CyclicLogSinkST>(1024 * 100);
    memory_sink->set_level(spdlog::level::debug);
    log_sinks.push_back(memory_sink);

    // Initialize platform (it can inject its log sinks)
    platform->Initialize(this);

    // Create core logger and use it as default logger
    log = CreateLogger("core");
    spdlog::set_default_logger(log);

    gpu = platform->CreateRenderBackend("DirectX11");
    renderer = std::make_unique<Renderer>(platform, gpu.get(), CreateLogger("renderer"));
}

Engine::~Engine()
{
}

void Engine::Run()
{
    using namespace std::chrono;

    PreRenderLoop();

    auto previous = high_resolution_clock::now();
    FrameInfo info;

    while (!IsTerminated()) {
        // Record when frame processing starts and calculate budget
        milliseconds time_budget;
        if (desiredFPS)
            time_budget = duration_cast<milliseconds>(seconds(1)) / 60;
        
        auto start = high_resolution_clock::now();

        OnFrameStart();

        if (IsTerminated())
            break;

        if (!IsPaused()) {
            info.frame++;
            info.deltaTime = duration<float, std::ratio<1>>(start - previous).count();

            Frame(info);
        }

        OnFrameEnd();

        previous = start;

        // FPS throttling - calculate how long should we sleep
        if (desiredFPS)
            std::this_thread::sleep_until(start + time_budget);
    }

    PostRenderLoop();
}

void Engine::PreRenderLoop()
{
    platform->PreRenderLoop(this);
    gpu->PreRenderLoop(this);
    simulation->PreRenderLoop(this);

    input = CreateLayer<InputLayer>();
    debug = CreateLayer<ImGuiLayer>();
}

void Engine::PostRenderLoop()
{
    while (!layers.empty())
        PopLayer();

    simulation->PostRenderLoop(this);
    gpu->PostRenderLoop(this);
    platform->PostRenderLoop(this);
}

void Engine::OnFrameStart()
{
    SPDLOG_LOGGER_DEBUG(log, "Frame start");

    platform->OnFrameStart(this);
    gpu->OnFrameStart(this);

    for (auto i = layers.begin(); i != layers.end(); i++)
        i->get()->OnFrameStart(this);
}

void Engine::OnFrameEnd()
{
    SPDLOG_LOGGER_DEBUG(log, "Frame end");

    // Draw everything
    renderer->DrawScene(renderer->GetScene());

    for (auto i = layers.rbegin(); i != layers.rend(); i++)
        i->get()->OnFrameEnd(this);

    gpu->OnFrameEnd(this);
    platform->OnFrameEnd(this);

    // Display frame
    gpu->Display();

    // Do it now to save time later
    gpu->ClearRenderTarget(gpu->GetScreenRenderTarget().get());
}

Layer *Engine::PushLayer(std::unique_ptr<Layer> layer)
{
    layers.push_back(std::move(layer));
    Layer* result = layers.back().get();

    result->OnAttach(this);
    return result;
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

void Engine::Frame(const FrameInfo& frame)
{
    simulation->Frame(this, frame);
}

void Engine::SubmitEvent(const Event& event)
{
    switch (event.type) {
    case Event::Type::MouseMoved:
        SPDLOG_LOGGER_DEBUG(log, "Event: {}", event.DebugDescription());
        break;
    case Event::Type::ApplicationResized:
        SPDLOG_LOGGER_INFO(log, "Event: {}", event.DebugDescription());
        renderer->OnScreenResize(event.ApplicationSize().first, event.ApplicationSize().second);
        break;
    default:
        SPDLOG_LOGGER_INFO(log, "Event: {}", event.DebugDescription());
        break;
    }

    for (auto i = layers.begin(); i != layers.end(); i++) {
        if (!i->get()->OnEvent(this, &event))
            break;
    }
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
