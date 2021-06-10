#pragma once

namespace DXP
{

struct DepthStencilLoader : Loader<DepthStencilLoader, DepthStencilTestDescription, DepthStencilTest>
{
    DepthStencilLoader(RenderBackend* gpu) : gpu(gpu) { };

    std::shared_ptr<DepthStencilTest> LoadResource(const DepthStencilTestDescription& desc);

    RenderBackend* gpu;
};


};
