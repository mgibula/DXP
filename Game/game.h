#pragma once

namespace Simworld
{

struct Game : public DXP::Simulation
{
    virtual void PreRenderLoop(DXP::Engine* engine) override;
    virtual void PostRenderLoop(DXP::Engine* engine) override;

private:
    std::shared_ptr<spdlog::logger> log;
};

};
