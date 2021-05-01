#include "pch.h"

namespace DXP
{

RenderObject::RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) : 
    mesh(mesh), 
    material(material),
    constantBufferPerObject(material->CreateConstantBufferData(ConstantBufferSlot::CB_Object))
{
    
}

};
