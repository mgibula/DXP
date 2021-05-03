#pragma once

namespace DXP
{

struct Camera : public SceneNode
{
    Camera();

    void SetPerspective(float scale = 1.f, float nearZ = 0.1f, float farZ = 1000.f);
    void SetOrthographic(float scale = 1.f, float nearZ = 0.1f, float farZ = 1000.f);

    void LookAt(float x, float y, float z);

    DirectX::XMMATRIX GetViewMatrix();
    DirectX::XMMATRIX GetProjectionMatrix(int width, int height);

    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;

    bool is_perspective_camera = false;
    float scale = 1.f;
    float nearZ = 0.1f;
    float farZ = 1000.f;
};

};
