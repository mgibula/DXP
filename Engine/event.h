#pragma once

namespace DXP
{

struct Event
{
    enum Category {
        Input           = 1,
        Mouse           = 2,
        Keyboard        = 4,
        Application     = 8,
    };

    enum class Type {
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        KeyPressed,
        KeyReleased,
        ApplicationMinimized,
        ApplicationResized,
    };

    enum MouseButton {
        Left    = 0,
        Middle  = 1,
        Right   = 2,
        // Additional numbers are allowed
    };

    Event(Type type, int category) : type(type), category(category) { };

    static Event MouseButtonPressed(int button);
    static Event MouseButtonReleased(int button);
    static Event MouseMoved(int x, int y);
    static Event MouseScrolled(int x, int y);
    static Event KeyPressed(int keycode);
    static Event KeyReleased(int keycode);
    static Event ApplicationMinimized();
    static Event ApplicationResized(int width, int height);

    int category;
    Type type;

    // For debugging/logging
    std::string DebugDescription() const;

    int MouseButton() const;
    std::pair<int, int> MousePosition() const;
    std::pair<int, int> MouseWheelDelta() const;
    int KeyCode() const;
    std::pair<int, int> ApplicationSize() const;

    union {
        struct {
            union {
                struct {
                    int button;
                } button;

                struct {
                    int x;
                    int y;
                } move;

                struct {
                    int x;
                    int y;
                } scroll;
            };
        } mouse;

        struct {
            int code;
        } key;

        struct {
            union {
                struct {
                    int width;
                    int height;
                } size;
            };
        } application;
    } params;
};

inline int Event::MouseButton() const
{
    DXP_ASSERT(type == Event::Type::MouseButtonPressed || type == Event::Type::MouseButtonReleased, "Event is not mouse button event");
    return params.mouse.button.button;
}

inline std::pair<int, int> Event::MousePosition() const
{
    DXP_ASSERT(type == Event::Type::MouseMoved, "Event is not mouse move event");
    return { params.mouse.move.x, params.mouse.move.y };
}

inline std::pair<int, int> Event::MouseWheelDelta() const
{
    DXP_ASSERT(type == Event::Type::MouseScrolled, "Event is not mouse scroll event");
    return { params.mouse.scroll.x, params.mouse.scroll.y };
}

inline int Event::KeyCode() const
{
    DXP_ASSERT(type == Event::Type::KeyPressed || type == Event::Type::KeyReleased, "Event is not keyboard event");
    return params.key.code;
}

inline std::pair<int, int> Event::ApplicationSize() const
{
    DXP_ASSERT(type == Event::Type::ApplicationResized, "Event is not application resized event");
    return { params.application.size.width, params.application.size.height };
}

};
