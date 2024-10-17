#include "key_event.h"
#include <sstream>

namespace Storytime {
    KeyEvent::KeyEvent(EventType type, const std::string& name, i32 key_code)
        : Event(type, name), key_code(key_code) {
    }

    KeyEvent::KeyEvent(EventType type, const std::string& name, i32 key_code, i32 mods, i32 scan_code)
        : Event(type, name), key_code(key_code), mods(mods), scan_code(scan_code) {
    }

    std::string KeyEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{key_code=" << key_code << "}";
        return ss.str();
    }

    const EventType KeyPressedEvent::type = 3975478234;
    const std::string KeyPressedEvent::name = "KeyPressedEvent";

    KeyPressedEvent::KeyPressedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, name, key_code, mods, scan_code) {
    }

    const EventType KeyReleasedEvent::type = 1829202745;
    const std::string KeyReleasedEvent::name = "KeyReleasedEvent";

    KeyReleasedEvent::KeyReleasedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, name, key_code, mods, scan_code) {
    }

    const EventType KeyRepeatedEvent::type = 1687606021;
    const std::string KeyRepeatedEvent::name = "KeyRepeatedEvent";

    KeyRepeatedEvent::KeyRepeatedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, name, key_code, mods, scan_code) {
    }

    const EventType KeyTypedEvent::type = 1418894294;
    const std::string KeyTypedEvent::name = "KeyTypedEvent";

    KeyTypedEvent::KeyTypedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, name, key_code, mods, scan_code) {
    }
}
