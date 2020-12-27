#pragma once

namespace DXP
{

struct RenderBackend
{
    virtual ~RenderBackend() = default;

    virtual bool Initialize() = 0;
};

};
