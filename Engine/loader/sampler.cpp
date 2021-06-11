#include "pch.h"

namespace DXP
{

std::shared_ptr<Sampler> SamplerLoader::LoadResource(const SamplerDescription& desc)
{
    return gpu->CreateSampler(desc);
}

};
