#pragma once

namespace DXP
{

struct RendererResources
{
    RendererResources(RenderBackend* gpu) :
        rasterizer(gpu),
        depthStencilTest(gpu)
    {
    }

    Codex<Sampler> sampler;

    RasterizerLoader rasterizer;
    DepthStencilLoader depthStencilTest;
};

};
