#include "st_key_released_event.h"

namespace Storytime {
    const EventType KeyReleasedEvent::type = 1829;
    const std::string KeyReleasedEvent::type_name = "KeyReleasedEvent";

    KeyReleasedEvent::KeyReleasedEvent() : Event(type, type_name) {
    }

    std::string KeyReleasedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{key_code=" << key_code << ", mods=" << mods << ", scan_code=" << scan_code << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyReleasedEvent& event) {
        return os << event.to_string();
    }
}