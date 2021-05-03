#include "pch.h"
#include "../Imgui/imgui.h"

using namespace DirectX;

namespace DXP
{

SceneNode::SceneNode()
{
    XMStoreFloat3(&position, XMVectorReplicate(0.f));
    XMStoreFloat3(&rotation, XMVectorReplicate(0.f));
    XMStoreFloat3(&scaling, XMVectorReplicate(1.f));
}

RenderObject::RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) : 
    mesh(mesh), 
    material(material),
    constantBufferPerObject(material->CreateConstantBufferData(ConstantBufferSlot::CB_Object))
{
    
}

void SceneNode::ImGuiDebug()
{
    int node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (ImGui::TreeNodeEx(name.c_str(), node_flags, "%s", name.c_str())) {
        int changed;
        float position[3] = { this->position.x, this->position.y, this->position.z };
        float degrees[3] = {
            XMConvertToDegrees(rotation.x),
            XMConvertToDegrees(rotation.y),
            XMConvertToDegrees(rotation.z)
        };
        float scale[3] = { this->scaling.x, this->scaling.y, this->scaling.z };

        changed = ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.1f);
        if (changed)
            MoveTo(position[0], position[1], position[2]);

        changed = ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&degrees), 1.f);
        if (changed)
            RotateTo(degrees[0], degrees[1], degrees[2]);

        changed = ImGui::DragFloat3("Scaling", reinterpret_cast<float*>(&scale), .1f);
        if (changed)
            ScaleTo(scale[0], scale[1], scale[2]);

        for (const auto& child : children) {
            child->ImGuiDebug();
        }

        ImGui::TreePop();
    }
}

};
