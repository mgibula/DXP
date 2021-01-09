#pragma once

namespace DXP
{

struct ImGuiLayer final : public Layer
{
    virtual void OnAttach(Engine* engine) override;
    virtual void OnDetach(Engine* engine) override;

    virtual void OnFrameStart(Engine* engine) override;

    virtual void OnFrameEnd(Engine* engine) override;

    virtual void OnImguiFrame(Engine* engine) override;
};

};
