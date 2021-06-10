#include "pch.h"

namespace DXP
{

std::shared_ptr<DepthStencilTest> DepthStencilLoader::LoadResource(const DepthStencilTestDescription& desc)
{
    return gpu->CreateDepthStencilTest(desc);
}

};
