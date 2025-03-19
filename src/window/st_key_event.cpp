#include "st_key_event.h"
#include <sstream>

namespace Storytime {
    KeyEvent::KeyEvent(EventType event_type, const std::string& event_type_name)
        : Event(event_type, event_type_name) {
    }

    KeyEvent::KeyEvent(EventType event_type, const std::string& event_type_name, i32 key_code)
        : Event(event_type, event_type_name), key_code(key_code) {
    }

    KeyEvent::KeyEvent(EventType event_type, const std::string& event_type_name, i32 key_code, i32 mods, i32 scan_code)
        : Event(event_type, event_type_name), key_code(key_code), mods(mods), scan_code(scan_code) {
    }

    std::string KeyEvent::to_string() const {
        std::stringstream ss;
        ss << event_type_name << "{key_code=" << key_code << "}";
        return ss.str();
    }

    const EventType KeyPressedEvent::type = 3975;
    const std::string KeyPressedEvent::type_name = "KeyPressedEvent";

    KeyPressedEvent::KeyPressedEvent()
        : KeyEvent(type, type_name) {
    }

    KeyPressedEvent::KeyPressedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, type_name, key_code, mods, scan_code) {
    }

    const EventType KeyReleasedEvent::type = 1829;
    const std::string KeyReleasedEvent::type_name = "KeyReleasedEvent";

    KeyReleasedEvent::KeyReleasedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, type_name, key_code, mods, scan_code) {
    }

    const EventType KeyRepeatedEvent::type = 1687;
    const std::string KeyRepeatedEvent::type_name = "KeyRepeatedEvent";

    KeyRepeatedEvent::KeyRepeatedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, type_name, key_code, mods, scan_code) {
    }

    const EventType KeyTypedEvent::type = 1418;
    const std::string KeyTypedEvent::type_name = "KeyTypedEvent";

    KeyTypedEvent::KeyTypedEvent(i32 key_code, i32 mods, i32 scan_code)
        : KeyEvent(type, type_name, key_code, mods, scan_code) {
    }
}
