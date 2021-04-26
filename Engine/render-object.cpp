#include "pch.h"

namespace DXP
{

RenderObject::RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) : 
    mesh(mesh), 
    material(material),
    constantBufferPerObject(material->vertexShader->GetConstantBufferLayout(ConstantBufferSlot::Object))
{
    
}

void ConstantBufferValues::Set(std::string_view name, float value)
{
    values.insert(std::make_pair(name, value));
}

};
