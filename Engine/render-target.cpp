#include "pch.h"

namespace DXP
{

void RendererOutput::Resize(RenderBackend* gpu, int width, int height)
{
    gpu->ResizeRenderTarget(renderTarget.get(), width, height);

    if (depthStencilTexture)
        depthStencilTexture = gpu->CreateDepthStencilTexture(width, height);

    viewport = gpu->CreateViewport(0, 0, width, height);
}

};
