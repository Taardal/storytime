#include "KeyEvent.h"
#include <sstream>

namespace Storytime {
    KeyEvent::KeyEvent(std::string_view name, EventType type, int32_t keyCode)
        : Event(name, type), keyCode(keyCode) {
    }

    KeyEvent::KeyEvent(const std::string_view& name, EventType type, int32_t key_code, int32_t mods, int32_t scan_code)
        : Event(name, type),
          keyCode(key_code),
          mods(mods),
          scanCode(scan_code) {
    }

    int32_t KeyEvent::GetKeyCode() const {
        return keyCode;
    }

    std::string KeyEvent::ToString() const {
        std::stringstream ss;
        ss << name << "{keyCode=" << keyCode << "}";
        return ss.str();
    }

    KeyPressedEvent::KeyPressedEvent(int32_t key_code, int32_t mods, int32_t scan_code)
        : KeyEvent("KeyPressedEvent", EventType::KeyPressed, key_code, mods, scan_code) {
    }

    KeyReleasedEvent::KeyReleasedEvent(int32_t key_code, int32_t mods, int32_t scan_code)
        : KeyEvent("KeyReleasedEvent", EventType::KeyReleased, key_code, mods, scan_code) {
    }

    KeyRepeatedEvent::KeyRepeatedEvent(int32_t key_code, int32_t mods, int32_t scan_code)
        : KeyEvent("KeyRepeatedEvent", EventType::KeyRepeated, key_code, mods, scan_code) {
    }

    KeyTypedEvent::KeyTypedEvent(int32_t key_code, int32_t mods, int32_t scan_code)
        : KeyEvent("KeyTypedEvent", EventType::KeyTyped, key_code, mods, scan_code) {
    }
}
