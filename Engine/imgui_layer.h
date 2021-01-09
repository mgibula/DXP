#pragma once

namespace DXP
{

struct ImGuiLayer final : public Layer
{
    virtual void OnAttach(Engine* engine) override;
    virtual void OnDetach(Engine* engine) override;

    virtual void OnFrameStart(Engine* engine) override;
    virtual void OnFrameEnd(Engine* engine) override;

    virtual void OnImguiFrame(Engine* engine) override;
    virtual bool OnEvent(Engine* engine, const Event* event) override;

private:
    static constexpr int counters = 60 * 5;
    RingBuffer<float32_t, counters> processingTimes;
    std::chrono::high_resolution_clock::time_point started;
};

};
