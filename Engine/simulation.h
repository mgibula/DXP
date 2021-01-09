#pragma once

namespace DXP
{

struct Engine;
struct FrameInfo;

struct Simulation
{
    virtual ~Simulation() = default;

    virtual void PreRenderLoop(Engine* engine) { };
    virtual void PostRenderLoop(Engine* engine) { };
    virtual void Frame(Engine* engine, const FrameInfo& frame) { };
};

};
