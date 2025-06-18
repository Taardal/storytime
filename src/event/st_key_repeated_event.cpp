#include "st_key_repeated_event.h"

namespace Storytime {
    const EventType KeyRepeatedEvent::type = 1687;
    const std::string KeyRepeatedEvent::type_name = "KeyRepeatedEvent";

    KeyRepeatedEvent::KeyRepeatedEvent() : Event(type, type_name) {
    }

    std::string KeyRepeatedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{key_code=" << key_code << ", mods=" << mods << ", scan_code=" << scan_code << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyRepeatedEvent& event) {
        return os << event.to_string();
    }
}