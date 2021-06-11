#pragma once

namespace DXP
{

struct RendererResources
{
    RendererResources(RenderBackend* gpu) :
        rasterizer(gpu),
        depthStencilTest(gpu),
        sampler(gpu)
    {
    }

    RasterizerLoader rasterizer;
    DepthStencilLoader depthStencilTest;
    SamplerLoader sampler;
};

};
