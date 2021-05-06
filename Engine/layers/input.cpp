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

    result.x = currentPosition.x / static_cast<float>(engine->renderer->Width());
    result.y = currentPosition.y / static_cast<float>(engine->renderer->Height());

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
        XMMATRIX projectionMatrix = camera->GetProjectionMatrix(engine->renderer->Width(), engine->renderer->Height());

        auto screenPosition = GetMousePosition();
        XMFLOAT3 screenPositionFloat;
        screenPositionFloat.x = screenPosition.x;
        screenPositionFloat.y = screenPosition.y;
        screenPositionFloat.z = 0.f;

        auto mouseInWorld = XMVector3Unproject(XMVectorSet(screenPosition.x, screenPosition.y, 0.0f, 1.0f), 0.f, 0.f, engine->renderer->Width(), engine->renderer->Height(), camera->nearZ, camera->farZ, projectionMatrix, viewMatrix, XMMatrixIdentity());
        XMStoreFloat3(&screenPositionFloat, mouseInWorld);

        FormatToCString(buffer, "Mouse position: {}, {}, {}, {}", currentPosition.x, currentPosition.y, screenPositionFloat.x, screenPositionFloat.y);
        ImGui::Text(buffer.data());
    }


    ImGui::End();
}

};
