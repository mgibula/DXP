#include "pch.h"
#include "utils.h"
#include "event.h"

namespace DXP
{

std::string Event::DebugDescription() const
{
    switch (type) {
    case Type::MouseButtonPressed:
        return fmt::format("MouseButtonPressed({})", MouseButton());
    case Type::MouseButtonReleased:
        return fmt::format("MouseButtonReleased({})", MouseButton());
    case Type::MouseMoved:
        return fmt::format("MouseMoved({}, {})", MousePosition().first, MousePosition().second);
    case Type::MouseScrolled:
        return fmt::format("MouseScrolled({}, {})", MouseWheelDelta().first, MouseWheelDelta().second);
    case Type::KeyPressed:
        return fmt::format("KeyPressed({})", KeyCode());
    case Type::KeyReleased:
        return fmt::format("KeyReleased({})", KeyCode());
    case Type::ApplicationMinimized:
        return fmt::format("ApplicationMinimized()");
    case Type::ApplicationResized:
        return fmt::format("ApplicationResized({}, {})", ApplicationSize().first, ApplicationSize().second);
    default:
        Fatal("Unsupported event type");
    }
}

Event Event::MouseButtonPressed(int button)
{
    Event result{ Event::Type::MouseButtonPressed, Event::Category::Input | Event::Category::Mouse };
    result.params.mouse.button.button = button;

    return result;
}

Event Event::MouseButtonReleased(int button)
{
    Event result{ Event::Type::MouseButtonReleased, Event::Category::Input | Event::Category::Mouse };
    result.params.mouse.button.button = button;

    return result;
}

Event Event::MouseMoved(int x, int y)
{
    Event result{ Event::Type::MouseMoved, Event::Category::Input | Event::Category::Mouse };
    result.params.mouse.move.x = x;
    result.params.mouse.move.y = y;

    return result;
}

Event Event::MouseScrolled(int x, int y)
{
    Event result{ Event::Type::MouseScrolled, Event::Category::Input | Event::Category::Mouse };
    result.params.mouse.scroll.x = x;
    result.params.mouse.scroll.y = y;

    return result;
}

Event Event::KeyPressed(int keycode)
{
    Event result{ Event::Type::KeyPressed, Event::Category::Input | Event::Category::Keyboard };
    result.params.key.code = keycode;

    return result;
}

Event Event::KeyReleased(int keycode)
{
    Event result{ Event::Type::KeyReleased, Event::Category::Input | Event::Category::Keyboard };
    result.params.key.code = keycode;

    return result;
}

Event Event::ApplicationMinimized()
{
    Event result{ Event::Type::ApplicationMinimized, Event::Category::Input | Event::Category::Application };
    return result;
}

Event Event::ApplicationResized(int width, int height)
{
    Event result{ Event::Type::ApplicationResized, Event::Category::Input | Event::Category::Application };
    result.params.application.size.width = width;
    result.params.application.size.height = height;

    return result;
}

};
