#pragma once

namespace DXP
{

struct Engine;

struct Simulation
{
    virtual ~Simulation() = default;

    virtual void PreRenderLoop(Engine* engine) { };
    virtual void PostRenderLoop(Engine* engine) { };
};

};
