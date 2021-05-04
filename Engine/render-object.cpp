#include "pch.h"
#include "../Imgui/imgui.h"
#include <cmath>

using namespace DirectX;

namespace DXP
{

SceneNode::SceneNode()
{
    XMStoreFloat3(&position, XMVectorReplicate(0.f));
    XMStoreFloat4(&rotation, XMQuaternionIdentity());
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

        changed = ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position), 0.1f);
        if (changed)
            MoveTo(position[0], position[1], position[2]);

        // Conversion to eulers for debugging
        float x = rotation.x;
        float y = rotation.y;
        float z = rotation.z;
        float w = rotation.w;

        float pitch = std::atan2(2.f * x * w - 2.f * y * z, 1.f - 2.f * (x * x) - 2.f * (z * z));
        float roll = std::asin(-2.0 * (x * z - w * y));
        float yaw = std::atan2(2.f * (x * y + z * w), 1.f - 2.f * (y, y + z * z));

        float degrees[3], degrees_orig[3];
        degrees_orig[0] = degrees[0] = XMConvertToDegrees(pitch);
        degrees_orig[1] = degrees[1] = XMConvertToDegrees(roll);
        degrees_orig[2] = degrees[2] = XMConvertToDegrees(yaw);

        changed = ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&degrees), 1.f);
        if (changed)
            RotateBy(degrees[0] - degrees_orig[0], degrees[1] - degrees_orig[1], degrees[2] - degrees_orig[2]);

        float scale[3] = { this->scaling.x, this->scaling.y, this->scaling.z };
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
