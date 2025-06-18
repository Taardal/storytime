#include "../window/st_key_event.h"
#include <sstream>

namespace Storytime {
    const EventType KeyPressedEvent::type = 3975;
    const std::string KeyPressedEvent::type_name = "KeyPressedEvent";

    KeyPressedEvent::KeyPressedEvent() : Event(type, type_name) {
    }

    std::string KeyPressedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name;
        ss << "{";
        ss << "key_code=" << key_code;
        ss << ", ";
        ss << "mods=" << mods;
        ss << ", ";
        ss << "scan_code=" << scan_code;
        ss << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyPressedEvent& event) {
        return os << event.to_string();
    }
}

namespace Storytime {
    const EventType KeyReleasedEvent::type = 1829;
    const std::string KeyReleasedEvent::type_name = "KeyReleasedEvent";

    KeyReleasedEvent::KeyReleasedEvent() : Event(type, type_name) {
    }

    std::string KeyReleasedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name;
        ss << "{";
        ss << "key_code=" << key_code;
        ss << ", ";
        ss << "mods=" << mods;
        ss << ", ";
        ss << "scan_code=" << scan_code;
        ss << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyReleasedEvent& event) {
        return os << event.to_string();
    }
}

namespace Storytime {
    const EventType KeyRepeatedEvent::type = 1687;
    const std::string KeyRepeatedEvent::type_name = "KeyRepeatedEvent";

    KeyRepeatedEvent::KeyRepeatedEvent() : Event(type, type_name) {
    }

    std::string KeyRepeatedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name;
        ss << "{";
        ss << "key_code=" << key_code;
        ss << ", ";
        ss << "mods=" << mods;
        ss << ", ";
        ss << "scan_code=" << scan_code;
        ss << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyRepeatedEvent& event) {
        return os << event.to_string();
    }
}

namespace Storytime {
    const EventType KeyTypedEvent::type = 1418;
    const std::string KeyTypedEvent::type_name = "KeyTypedEvent";

    KeyTypedEvent::KeyTypedEvent() : Event(type, type_name) {
    }

    std::string KeyTypedEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name;
        ss << "{";
        ss << "key_code=" << key_code;
        ss << ", ";
        ss << "mods=" << mods;
        ss << ", ";
        ss << "scan_code=" << scan_code;
        ss << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const KeyTypedEvent& event) {
        return os << event.to_string();
    }
}
