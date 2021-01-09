#pragma once

namespace DXP
{

struct Engine;
struct Event;

struct Layer
{
    virtual ~Layer() = default;

    // This function is called after layer has been attached
    virtual void OnAttach(Engine* engine) { };

    // This function is called after layer has been detached
    virtual void OnDetach(Engine* engine) { };

    // This function is called (in order of registration) just after frame generation has started
    virtual void OnFrameStart(Engine* engine) { };

    // This function is called (in REVERSED order of registration) just after frame generation has ended
    virtual void OnFrameEnd(Engine* engine) { };

    // This function is called (in order of registration) if debug layer is active and after frame generation has ended
    virtual void OnImguiFrame(Engine* engine) { };

    // Called when event is received (in REVERSED order of registration) at unspecified time
    // TODO - should events be buffered and submitted at known time?
    //
    // Return value indicates if event should continue propagating
    virtual bool OnEvent(Engine* engine, const Event* event) { return true; };
};

};
