#pragma once

namespace DXP
{

struct RenderTarget
{
    virtual ~RenderTarget() = default;
};

struct Viewport
{
    virtual ~Viewport() = default;

    virtual int GetX() const = 0;

    virtual int GetY() const = 0;

    virtual int GetWidth() const = 0;

    virtual int GetHeight() const = 0;
};

};
