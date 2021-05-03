#pragma once

namespace DXP
{

struct Renderer;
struct Camera;

struct SceneNode
{
    SceneNode();

    virtual ~SceneNode() = default;

    virtual void ImGuiDebug();

    template <typename T, typename... Args>
    T* AddChild(Args... args) {
        T* result = static_cast<T *>(children.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get());
        result->parent = this;

        return result;
    };

    void SetName(std::string_view name) {
        this->name = name;
    };

    void MoveTo(float x, float y, float z) {
        using namespace DirectX;
        position = XMFLOAT3(x, y, z);
    };

    void MoveBy(float x, float y, float z) {
        using namespace DirectX;
        XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), XMVectorSet(x, y, z, 0.f)));
    };

    void RotateTo(float pitch, float yaw, float roll) {
        using namespace DirectX;
        rotation = XMFLOAT3(
            XMConvertToRadians(pitch),
            XMConvertToRadians(yaw),
            XMConvertToRadians(roll)
        );
    };

    void ScaleTo(float x, float y, float z) {
        using namespace DirectX;
        scaling = XMFLOAT3(x, y, z);
    };

    void ScaleBy(float x, float y, float z) {
        using namespace DirectX;
        XMStoreFloat3(&scaling, XMVectorAdd(XMLoadFloat3(&scaling), XMVectorSet(x, y, z, 0.f)));
    };

    DirectX::XMMATRIX GetWorldMatrix() const {
        using namespace DirectX;

        return  XMMatrixTranslationFromVector(XMLoadFloat3(&position)) *
            XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)) *
            XMMatrixScaling(scaling.x, scaling.y, scaling.z);
    };

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scaling;

    std::string name;

    SceneNode* parent = nullptr;
    std::vector<std::unique_ptr<SceneNode>> children;
};

struct SceneRoot : public SceneNode
{
    SceneRoot() {
        SetName("[root]");
    };

    void SetMainCamera(Camera* camera) {
        mainCamera = camera;
    };

    Camera* mainCamera = nullptr;
};

struct RenderObject : public SceneNode
{
    RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    
    std::unique_ptr<ConstantBufferContent> constantBufferPerObject;
};

};
