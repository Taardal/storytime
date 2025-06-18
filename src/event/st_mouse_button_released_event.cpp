#include "st_mouse_button_released_event.h"

namespace Storytime {
    const EventType MouseButtonReleasedEvent::type = 4203;
    const std::string MouseButtonReleasedEvent::type_name = "MouseButtonReleasedEvent";

    MouseButtonReleasedEvent::MouseButtonReleasedEvent() : Event(type, type_name) {
    }

    std::string MouseButtonReleasedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{button=" << button << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const MouseButtonReleasedEvent& event) {
        return os << event.to_string();
    }
}
