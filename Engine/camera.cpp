#include "pch.h"

namespace DXP
{

using namespace DirectX;

Camera::Camera()
{
    XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
    XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(XMMatrixIdentity()));
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

DirectX::XMMATRIX Camera::GetProjectionMatrix(int width, int height)
{
    if (is_perspective_camera) {
        return XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, (float) width / height, nearZ, farZ);
        //return XMMatrixPerspectiveLH(scale, (float) height / width * scale, nearZ, farZ);
    } else {
        return XMMatrixOrthographicLH(scale, (float) height / width * scale, nearZ, farZ);
    }
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
    return XMLoadFloat4x4(&viewMatrix);
}

void Camera::LookAt(float x, float y, float z)
{
    //XMFLOAT3 cameraPosition{ 0.f, 0.f, 0.f };
    XMFLOAT3 cameraPosition{ position.x, position.y, position.z };
    XMFLOAT3 focalPoint{ x, y, z };
    XMFLOAT3 upDirection{ 0.f, 1.f, 1.f };

    XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&cameraPosition), XMLoadFloat3(&focalPoint), XMLoadFloat3(&upDirection));
    XMStoreFloat4x4(&viewMatrix, view);
}

};
