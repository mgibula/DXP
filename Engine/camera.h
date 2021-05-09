#pragma once

namespace DXP
{

struct Camera final : public SceneNode
{
    Camera();

    void SetPerspective(float scale = 1.f, float nearZ = 0.1f, float farZ = 100.f);
    void SetOrthographic(float scale = 1.f, float nearZ = 0.1f, float farZ = 100.f);

    void LookAt(float x, float y, float z);

    DirectX::XMMATRIX GetViewMatrix();
    DirectX::XMMATRIX GetProjectionMatrix(int width, int height);

    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;

    bool is_perspective_camera = false;
    float scale = 1.f;
    float nearZ = 0.1f;
    float farZ = 30.f;

    DirectX::XMFLOAT3 target;
    bool drawDebug = false;

protected:
    virtual void ImGuiDebugComponent(Engine* engine) override;
};

};
