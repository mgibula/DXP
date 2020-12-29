#include "pch.h"
#include "utils.h"
#include "event.h"

namespace DXP
{

Event Event::MouseButtonPressed(int button)
{
    Event result{ Event::Type::MouseButtonPressed, Event::Category::Input | Event::Category::Mouse };
    result.mouse.button.button = button;

    return result;
}

Event Event::MouseMoved(int x, int y)
{
    Event result{ Event::Type::MouseMoved, Event::Category::Input | Event::Category::Mouse };
    result.mouse.move.x = x;
    result.mouse.move.y = y;

    return result;
}

Event Event::MouseScrolled(int delta)
{
    Event result{ Event::Type::MouseScrolled, Event::Category::Input | Event::Category::Mouse };
    result.mouse.scroll.delta = delta;

    return result;
}

Event Event::KeyPressed(int keycode)
{
    Event result{ Event::Type::KeyPressed, Event::Category::Input | Event::Category::Keyboard };
    result.key.code = keycode;

    return result;
}

Event Event::KeyReleased(int keycode)
{
    Event result{ Event::Type::KeyReleased, Event::Category::Input | Event::Category::Keyboard };
    result.key.code = keycode;

    return result;
}

};
