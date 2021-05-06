#pragma once

namespace DXP
{

struct InputLayer final : public Layer
{
    virtual void OnAttach(Engine* engine) override; 

    virtual bool OnEvent(Engine* engine, const Event* event) override;

    virtual void OnImguiFrame(Engine* engine) override;

    DirectX::XMINT2 GetMousePosition() const {
        return currentPosition;
    };

    DirectX::XMFLOAT2 GetMousePositionScreenSpace() const;

private:
    Engine* engine = nullptr;
    DirectX::XMINT2 currentPosition;
};

};
