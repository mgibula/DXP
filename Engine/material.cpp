#include "pch.h"

namespace DXP
{

Material::Material(std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader) :
    vertexShader(vertexShader),
    pixelShader(pixelShader),
    constantBufferPerMaterial(vertexShader->GetConstantBufferLayout(ConstantBufferSlot::Material))
{
}

};
