#include "st_mouse_scroll_event.h"

namespace Storytime {
    const EventType MouseScrollEvent::type = 1065;
    const std::string MouseScrollEvent::type_name = "MouseScrollEvent";

    MouseScrollEvent::MouseScrollEvent() : Event(type, type_name) {
    }

    std::string MouseScrollEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{x_offset=" << x_offset << ", y_offset=" << y_offset << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const MouseScrollEvent& event) {
        return os << event.to_string();
    }
}
