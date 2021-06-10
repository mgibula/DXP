#pragma once

namespace DXP
{

struct RasterizerLoader : Loader<RasterizerLoader, RasterizerDescription, Rasterizer>
{
    RasterizerLoader(RenderBackend* gpu) : gpu(gpu) { };

    std::shared_ptr<Rasterizer> LoadResource(const RasterizerDescription& desc);

    RenderBackend* gpu;
};


};
