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

    SceneNode* parent = nullptr;
    std::vector<std::unique_ptr<SceneNode>> children;
};

struct RenderObject : public SceneNode
{
    RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

    ConstantBufferContent* ShaderVariables() {
        return &constantBufferPerObject;
    };
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    
    ConstantBufferContent constantBufferPerObject;
};

};
