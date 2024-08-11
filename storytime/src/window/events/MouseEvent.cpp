#include "MouseEvent.h"
#include <sstream>

namespace Storytime
{
    MouseMovedEvent::MouseMovedEvent(float x, float y)
            : Event("MouseMovedEvent", EventType::MouseMoved), x(x), y(y)
    {
    }

    float MouseMovedEvent::GetX() const
    {
        return x;
    }

    float MouseMovedEvent::GetY() const
    {
        return y;
    }

    std::string MouseMovedEvent::ToString() const
    {
        std::stringstream ss;
        ss << name << "{x=" << x << ", y=" << y << "}";
        return ss.str();
    }

    MouseScrollEvent::MouseScrollEvent(float xOffset, float yOffset)
            : Event("MouseScrolledEvent", EventType::MouseScroll), xOffset(xOffset), yOffset(yOffset)
    {
    }

    float MouseScrollEvent::GetXOffset() const
    {
        return xOffset;
    }

    float MouseScrollEvent::GetYOffset() const
    {
        return yOffset;
    }

    std::string MouseScrollEvent::ToString() const
    {
        std::stringstream ss;
        ss << name << "{xOffset=" << xOffset << ", yOffset=" << yOffset << "}";
        return ss.str();
    }

    MouseButtonEvent::MouseButtonEvent(std::string_view name, EventType type, int32_t button)
            : Event(name, type), button(button)
    {
    }

    std::string MouseButtonEvent::ToString() const
    {
        std::stringstream ss;
        ss << name << "{button=" << button << "}";
        return ss.str();
    }

    int32_t MouseButtonEvent::GetButton() const
    {
        return button;
    }

    MouseButtonPressedEvent::MouseButtonPressedEvent(int32_t button)
            : MouseButtonEvent("MouseButtonPressedEvent", EventType::MouseButtonPressed, button)
    {
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(int32_t button)
            : MouseButtonEvent("MouseButtonReleasedEvent", EventType::MouseButtonReleased, button)
    {
    }
}
