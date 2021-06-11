#pragma once

namespace DXP
{

struct SamplerLoader : Loader<SamplerLoader, SamplerDescription, Sampler>
{
    SamplerLoader(RenderBackend* gpu) : gpu(gpu) { };

    std::shared_ptr<Sampler> LoadResource(const SamplerDescription& desc);

    RenderBackend* gpu;
};


};
