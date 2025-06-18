#include "st_key_typed_event.h"

namespace Storytime {
    const EventType KeyTypedEvent::type = 1418;
    const std::string KeyTypedEvent::type_name = "KeyTypedEvent";

    KeyTypedEvent::KeyTypedEvent() : Event(type, type_name) {
    }

    std::string KeyTypedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name  << "{key_code=" << key_code  << ", mods=" << mods  << ", scan_code=" << scan_code  << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyTypedEvent& event) {
        return os << event.to_string();
    }
}
