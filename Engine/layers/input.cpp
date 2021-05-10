#include "pch.h"
#include "input.h"
#include "../Imgui/imgui.h"

using namespace DirectX;

namespace DXP
{

void InputLayer::OnAttach(Engine* engine)
{
    this->engine = engine;
}

bool InputLayer::OnEvent(Engine* engine, const Event* event)
{
    if (event->type == Event::Type::MouseMoved) {
        std::tie(currentPosition.x, currentPosition.y) = event->MousePosition();
    }

    // Input layer is supposed to be last of "system" layers and it dispatches
    // all currently unprocessed events to the application code
    for (auto i = engine->layers.rbegin(); i != engine->layers.rend(); i++) {
        if (i->get() == this)
            break;

        if (!i->get()->OnEvent(engine, event))
            break;
    }

    return false;
}

XMFLOAT2 InputLayer::GetMousePositionScreenSpace() const
{
    XMFLOAT2 result;

    result.x = currentPosition.x / static_cast<float>(engine->platform->ScreenWidth());
    result.y = currentPosition.y / static_cast<float>(engine->platform->ScreenHeight());

    return result;
}

void InputLayer::OnImguiFrame(Engine* engine)
{
    static bool input_opened;
    static fmt::memory_buffer buffer;

    if (ImGui::Begin("Input", &input_opened))
    {
        auto* camera = engine->renderer->GetScene()->mainCamera;
        XMMATRIX viewMatrix = camera->GetViewMatrix();
        XMMATRIX projectionMatrix = camera->GetProjectionMatrix(engine->platform->ScreenWidth(), engine->platform->ScreenHeight());

        auto screenPosition = GetMousePosition();
        XMFLOAT3 screenPositionFloat;
        screenPositionFloat.x = 0.f; //screenPosition.x;
        screenPositionFloat.y = 0.f; // screenPosition.y;
        screenPositionFloat.z = 1.f; // / camera->farZ;

        //auto mouseInWorld = XMVector3Unproject(XMVectorSet(screenPosition.x, screenPosition.y, screenPosition.y, 0.0f), 0.f, 0.f, engine->renderer->Width(), engine->renderer->Height(), camera->nearZ, camera->farZ, projectionMatrix, viewMatrix, XMMatrixIdentity());
        //auto mouseInWorld = XMVector3Unproject(XMVectorSet(screenPosition.x, screenPosition.y, screenPosition.y, 0.0f), 0.f, 0.f, engine->renderer->Width(), engine->renderer->Height(), 0.f, 1.f, projectionMatrix, viewMatrix, XMMatrixIdentity());


        //XMVECTOR rayStart = XMVectorSet(screenPosition.x, screenPosition.y, 0.f, 0.f);
        //XMVECTOR rayEnd = XMVectorSet(screenPosition.x, screenPosition.y, 1.f, 0.f);
        //auto diff = XMVector3Normalize(rayEnd - rayStart);
        //auto mouseInWorld = XMVector3Unproject(diff, 0.f, 0.f, engine->renderer->Width(), engine->renderer->Height(), 0.f, 1.f, projectionMatrix, viewMatrix, XMMatrixIdentity());
        //auto mouseInWorld = XMVector3Unproject(diff, 0.f, 0.f, engine->renderer->Width(), engine->renderer->Height(), 0.f, 1.f, projectionMatrix, viewMatrix, XMMatrixIdentity());

        auto m1 = XMVector3Unproject(XMVectorSet(screenPosition.x, screenPosition.y, 0.f, 0.0f), 0.f, 0.f, engine->platform->ScreenWidth(), engine->platform->ScreenHeight(), 0.f, 1.f, projectionMatrix, viewMatrix, XMMatrixIdentity());
        auto m2 = XMVector3Unproject(XMVectorSet(screenPosition.x, screenPosition.y, 1.f, 0.0f), 0.f, 0.f, engine->platform->ScreenWidth(), engine->platform->ScreenHeight(), 0.f, 1.f, projectionMatrix, viewMatrix, XMMatrixIdentity());

        auto dif = XMVectorSubtract(m2, m1);
        dif = XMVector3Normalize(dif);

        auto cammat = camera->GetLocalToWorldMatrix();
        //auto dif2 = XMVector3Transform(dif, cammat);
        //dif = XMVector3RotateCoord(dif, cammat);

        XMStoreFloat3(&screenPositionFloat, dif * 10.f);

        //engine->debug->DrawLine({ 0.f, 0.f, 0.f }, screenPositionFloat, { 0.f, 1.f, 1.f });

        FormatToCString(buffer, "Mouse position: {}, {}, {}, {}", currentPosition.x, currentPosition.y, screenPositionFloat.x, screenPositionFloat.y);
        ImGui::Text(buffer.data());
    }


    ImGui::End();
}

};
