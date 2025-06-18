#include "st_mouse_button_pressed_event.h"

namespace Storytime {
    const EventType MouseButtonPressedEvent::type = 4202;
    const std::string MouseButtonPressedEvent::type_name = "MouseButtonPressedEvent";

    MouseButtonPressedEvent::MouseButtonPressedEvent() : Event(type, type_name) {
    }

    std::string MouseButtonPressedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{button=" << button << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const MouseButtonPressedEvent& event) {
        return os << event.to_string();
    }
}
