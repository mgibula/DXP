#pragma once

namespace DXP
{

struct Renderer;

struct SceneNode;

struct Scene
{
    std::unique_ptr<SceneNode> root;

};

struct SceneNode
{
    virtual ~SceneNode() = default;

    template <typename T, typename... Args>
    T* AddChild(Args... args) {
        T* result = static_cast<T *>(children.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get());
        result->parent = this;

        return result;
    };

    struct {
        float x = 0.f;
        float y = 0.f;
        float z = 0.f;
    } position;

    struct {
        float pitch = 0.f;
        float yaw = 0.f;
        float roll = 0.f;
    } rotation;

    struct {
        float x = 1.f;
        float y = 1.f;
        float z = 1.f;
    } scaling;

    void MoveTo(float x, float y, float z) {
        position.x = x;
        position.y = y;
        position.z = z;
    };

    void MoveBy(float x, float y, float z) {
        position.x += x;
        position.y += y;
        position.z += z;
    };

    void ScaleTo(float x, float y, float z) {
        scaling.x = x;
        scaling.y = y;
        scaling.z = z;
    };

    void ScaleBy(float x, float y, float z) {
        scaling.x += x;
        scaling.y += y;
        scaling.z += z;
    };

    DirectX::XMMATRIX GetWorldMatrix() const {
        using namespace DirectX;

        return  XMMatrixTranslation(position.x, position.y, position.z) *
            XMMatrixRotationRollPitchYaw(rotation.pitch, rotation.yaw, rotation.roll) *
            XMMatrixScaling(scaling.x, scaling.y, scaling.z);
    };

    SceneNode* parent = nullptr;
    std::vector<std::unique_ptr<SceneNode>> children;
};

struct RenderObject : public SceneNode
{
    RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    
    std::unique_ptr<ConstantBufferContent> constantBufferPerObject;
};

};
