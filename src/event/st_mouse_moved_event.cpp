#include "st_mouse_moved_event.h"

namespace Storytime {
    const EventType MouseMovedEvent::type = 3112;
    const std::string MouseMovedEvent::type_name = "MouseMovedEvent";

    MouseMovedEvent::MouseMovedEvent() : Event(type, type_name) {
    }

    std::string MouseMovedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{x=" << x << ", y=" << y << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const MouseMovedEvent& event) {
        return os << event.to_string();
    }
}
