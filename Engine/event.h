#pragma once

namespace DXP
{

struct Event
{
    enum Category {
        Input           = 1,
        Mouse           = 2,
        Keyboard        = 4,
    };

    enum class Type {
        MouseButtonPressed,
        MouseMoved,
        MouseScrolled,
        KeyPressed,
        KeyReleased,
    };

    enum MouseButton {
        Left    = 0,
        Middle  = 1,
        Right   = 2,
        // Additional numbers are allowed
    };

    Event(Type type, int category) : type(type), category(category) { };

    static Event MouseButtonPressed(int button);
    static Event MouseMoved(int x, int y);
    static Event MouseScrolled(int delta);
    static Event KeyPressed(int keycode);
    static Event KeyReleased(int keycode);

    int category;
    Type type;

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
                    int delta;
                } scroll;
            };
        } mouse;

        struct {
            int code;
        } key;
    };
};

};
