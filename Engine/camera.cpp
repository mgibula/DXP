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
        return XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, (float) width / height, nearZ, farZ);
    } else {
        return XMMatrixOrthographicLH(scale, (float) height / width * scale, nearZ, farZ);
    }
}

XMMATRIX Camera::GetViewMatrix(FXMMATRIX parent)
{
#if 0
    XMFLOAT3 upDirection{ 0.f, 1.f, 1.f };

    XMVECTOR cameraPosition = XMLoadFloat3(&position);
    cameraPosition = XMVector3Transform(cameraPosition, parent);

    XMVECTOR focalPoint = XMLoadFloat3(&target);
    //focalPoint = XMVector3Transform(focalPoint, GetWorldMatrix());

    return XMMatrixLookAtLH(cameraPosition, focalPoint, XMLoadFloat3(&upDirection));
#endif

    XMVECTOR forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    XMVECTOR right = XMVectorSet(1.f, 0.f, 0.f, 0.f);

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
    XMVECTOR cameraTarget = XMVector3TransformCoord(forward, rotationMatrix);
    cameraTarget = XMVector3Normalize(cameraTarget);

    right = XMVector3TransformCoord(right, rotationMatrix);
    forward = XMVector3TransformCoord(forward, rotationMatrix);
    XMVECTOR up = XMVector3Cross(forward, right);

    XMVECTOR cameraPosition = XMLoadFloat3(&position);
    cameraPosition = XMVector3TransformCoord(cameraPosition, parent);
    cameraTarget = cameraPosition + cameraTarget;

    return XMMatrixLookAtLH(cameraPosition, cameraTarget, up);
}

void Camera::LookAt(float x, float y, float z)
{
    XMStoreFloat3(&target, XMVectorSet(x, y, z, 0.f));
}

};
