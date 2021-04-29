#include "pch.h"

namespace DXP
{

RenderObject::RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) : 
    mesh(mesh), 
    material(material),
    constantBufferPerObject(material->vertexShader->GetConstantBufferLayout(ConstantBufferSlot::Object))
{
    
}

};
