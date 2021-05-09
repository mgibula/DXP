#include "pch.h"

namespace DXP
{

using namespace DirectX;

Camera::Camera()
{
    XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
    XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(XMMatrixIdentity()));
    XMStoreFloat3(&target, XMVectorReplicate(0.f));
}

void Camera::SetPerspective(float scale, float nearZ, float farZ)
{
    this->scale = scale;
    this->nearZ = nearZ;
    this->farZ = farZ;
    this->is_perspective_camera = true;
}

void Camera::SetOrthographic(float scale, float nearZ, float farZ)
{
    this->scale = scale;
    this->nearZ = nearZ;
    this->farZ = farZ;
    this->is_perspective_camera = false;
}

XMMATRIX Camera::GetProjectionMatrix(int width, int height)
{
    if (is_perspective_camera) {
        return XMMatrixPerspectiveFovLH(3.141592654f / 6.0f, (float) width / height, nearZ, farZ);
    } else {
        return XMMatrixOrthographicLH(scale, (float) height / width * scale, nearZ, farZ);
    }
}

XMMATRIX Camera::GetViewMatrix()
{
    XMVECTOR forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    XMVECTOR right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    XMVECTOR up;

    XMMATRIX worldMatrix = GetLocalToWorldMatrix();
    XMVECTOR scale, rotation, transform;

    XMMatrixDecompose(
        &scale,
        &rotation,
        &transform,
        worldMatrix
    );

    XMVECTOR cameraTarget = XMVectorAdd(forward, transform);
    cameraTarget = XMVector3Rotate(cameraTarget, rotation);

    right = XMVector3Rotate(right, rotation);
    forward = XMVector3Rotate(forward, rotation);
    up = XMVector3Cross(forward, right);

    return XMMatrixLookAtLH(transform, cameraTarget, up);
}

void Camera::LookAt(float x, float y, float z)
{
    XMStoreFloat3(&target, XMVectorSet(x, y, z, 0.f));
}

void Camera::ImGuiDebugComponent(Engine* engine)
{
    if (!drawDebug)
        return;

    float width = 1.f;
    float height = 0.56;

    float zRatio = farZ / nearZ;
    float nearX = width / 2.f;
    float nearY = height / 2.f;
    float farX = nearX * zRatio;
    float farY = nearY * zRatio;

    engine->debug->DrawLine({ -nearX, nearY, nearZ }, { nearX, nearY, nearZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ nearX, nearY, nearZ }, { nearX, -nearY, nearZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ nearX, -nearY, nearZ }, { -nearX, -nearY, nearZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ -nearX, -nearY, nearZ }, { -farX, farY, farZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ -farX, farY, farZ }, { farX, farY, farZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ farX, farY, farZ }, { farX, -farY, farZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ farX, -farY, farZ }, { -farX, -farY, farZ }, { 0.f, 1.f, 1.f });
    engine->debug->DrawLine({ -farX, -farY, farZ }, { -nearX, nearY, nearZ }, { 0.f, 1.f, 1.f });

}

};
