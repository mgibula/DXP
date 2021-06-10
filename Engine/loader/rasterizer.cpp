#include "pch.h"

namespace DXP
{

std::shared_ptr<Rasterizer> RasterizerLoader::LoadResource(const RasterizerDescription& desc)
{
    return gpu->CreateRasterizer(desc);
}

};
