#include "mouse_event.h"
#include <sstream>

namespace Storytime {
    const EventType MouseMovedEvent::type = 3112864401;
    const std::string MouseMovedEvent::name = "MouseMovedEvent";

    MouseMovedEvent::MouseMovedEvent(f32 x, f32 y)
        : Event(type, name), x(x), y(y) {
    }

    std::string MouseMovedEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{x=" << x << ", y=" << y << "}";
        return ss.str();
    }

    const EventType MouseScrollEvent::type = 1065579489;
    const std::string MouseScrollEvent::name = "MouseScrollEvent";

    MouseScrollEvent::MouseScrollEvent(f32 xOffset, f32 yOffset)
        : Event(type, name), x_offset(xOffset), y_offset(yOffset) {
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

    const EventType MouseButtonPressedEvent::type = 4202187401;
    const std::string MouseButtonPressedEvent::name = "MouseButtonPressedEvent";

    MouseButtonPressedEvent::MouseButtonPressedEvent(i32 button)
        : MouseButtonEvent(type, name, button) {
    }

    const EventType MouseButtonReleasedEvent::type = 1378168369;
    const std::string MouseButtonReleasedEvent::name = "MouseButtonReleasedEvent";

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(i32 button)
        : MouseButtonEvent(type, name, button) {
    }
}
