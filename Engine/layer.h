#pragma once

namespace DXP
{

struct Engine;

struct Layer
{
    virtual ~Layer() = default;

    virtual void OnAttach(Engine* engine) { };
    virtual void OnDetach(Engine* engine) { };
    virtual void OnFrame(Engine* engine) { };
    virtual void OnImguiFrame(Engine* engine) { };
};

};
