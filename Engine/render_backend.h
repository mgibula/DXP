#pragma once

namespace DXP
{

struct RenderBackend
{
    virtual ~RenderBackend() = default;

    virtual bool Initialize() = 0;

    virtual void PreRenderLoop() { };

    virtual void PostRenderLoop() { };

    virtual void OnFrameStart() { };

    virtual void OnFrameEnd() { };

    virtual void ClearScreen() = 0;

    virtual void Display() = 0;
};

};
