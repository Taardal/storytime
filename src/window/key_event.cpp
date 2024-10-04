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

    KeyPressedEvent::KeyPressedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(EventType::KeyPressed, "KeyPressedEvent", key_code, mods, scan_code) {
    }

    KeyReleasedEvent::KeyReleasedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(EventType::KeyReleased, "KeyReleasedEvent", key_code, mods, scan_code) {
    }

    KeyRepeatedEvent::KeyRepeatedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(EventType::KeyRepeated, "KeyRepeatedEvent", key_code, mods, scan_code) {
    }

    KeyTypedEvent::KeyTypedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(EventType::KeyTyped, "KeyTypedEvent", key_code, mods, scan_code) {
    }
}
