#include "st_mouse_event.h"
#include <sstream>

namespace Storytime {
    const EventType MouseMovedEvent::type = 3112;
    const std::string MouseMovedEvent::type_name = "MouseMovedEvent";

    MouseMovedEvent::MouseMovedEvent(f32 x, f32 y)
        : Event(type, type_name), x(x), y(y) {
    }

    std::string MouseMovedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{x=" << x << ", y=" << y << "}";
        return ss.str();
    }

    const EventType MouseScrollEvent::type = 1065;
    const std::string MouseScrollEvent::type_name = "MouseScrollEvent";

    MouseScrollEvent::MouseScrollEvent(f32 x_offset, f32 y_offset)
        : Event(type, type_name), x_offset(x_offset), y_offset(y_offset) {
    }

    std::string MouseScrollEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{x_offset=" << x_offset << ", y_offset=" << y_offset << "}";
        return ss.str();
    }

    MouseButtonEvent::MouseButtonEvent(EventType type, const std::string& name, i32 button)
        : Event(type, name), button(button) {
    }

    std::string MouseButtonEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{button=" << button << "}";
        return ss.str();
    }

    const EventType MouseButtonPressedEvent::type = 4202;
    const std::string MouseButtonPressedEvent::type_name = "MouseButtonPressedEvent";

    MouseButtonPressedEvent::MouseButtonPressedEvent(i32 button)
        : MouseButtonEvent(type, type_name, button) {
    }

    const EventType MouseButtonReleasedEvent::type = 1378;
    const std::string MouseButtonReleasedEvent::type_name = "MouseButtonReleasedEvent";

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(i32 button)
        : MouseButtonEvent(type, type_name, button) {
    }
}
