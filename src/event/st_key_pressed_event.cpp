#include "st_key_pressed_event.h"

namespace Storytime {
    const EventType KeyPressedEvent::type = 3975;
    const std::string KeyPressedEvent::type_name = "KeyPressedEvent";

    KeyPressedEvent::KeyPressedEvent() : Event(type, type_name) {
    }

    std::string KeyPressedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{key_code=" << key_code << ", mods=" << mods << ", scan_code=" << scan_code << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyPressedEvent& event) {
        return os << event.to_string();
    }
}