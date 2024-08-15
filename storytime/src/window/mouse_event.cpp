#include "mouse_event.h"
#include <sstream>

namespace Storytime {
    MouseMovedEvent::MouseMovedEvent(f32 x, f32 y)
        : Event(EventType::MouseMoved, "MouseMovedEvent"), x(x), y(y) {
    }

    std::string MouseMovedEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{x=" << x << ", y=" << y << "}";
        return ss.str();
    }

    MouseScrollEvent::MouseScrollEvent(f32 xOffset, f32 yOffset)
        : Event(EventType::MouseScroll, "MouseScrollEvent"), x_offset(xOffset), y_offset(yOffset) {
    }

    std::string MouseScrollEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{x_offset=" << x_offset << ", y_offset=" << y_offset << "}";
        return ss.str();
    }

    MouseButtonEvent::MouseButtonEvent(EventType type, const std::string& name, i32 button)
        : Event(type, name), button(button) {
    }

    std::string MouseButtonEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{button=" << button << "}";
        return ss.str();
    }

    MouseButtonPressedEvent::MouseButtonPressedEvent(i32 button)
        : MouseButtonEvent(EventType::MouseButtonPressed, "MouseButtonPressedEvent", button) {
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(i32 button)
        : MouseButtonEvent(EventType::MouseButtonReleased, "MouseButtonReleasedEvent", button) {
    }
}
