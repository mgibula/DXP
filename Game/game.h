#pragma once

namespace Simworld
{

struct Game final : public DXP::Simulation
{
    virtual void PreRenderLoop(DXP::Engine* engine) override;
    virtual void PostRenderLoop(DXP::Engine* engine) override;
    virtual void Frame(DXP::Engine* engine, const DXP::FrameInfo& frame) override;

private:
    std::shared_ptr<spdlog::logger> log;
};

};
