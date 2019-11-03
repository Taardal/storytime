#include "KeyEvent.h"
#include <sstream>

namespace Darkle {

    KeyEvent::KeyEvent(std::string_view name, EventType type, unsigned int keyCode) : Event(name, type), keyCode(keyCode) {
    }

    unsigned int KeyEvent::getKeyCode() const {
        return keyCode;
    }

    std::string KeyEvent::toString() const {
        std::stringstream ss;
        ss << name << "{keyCode=" << keyCode << "}";
        return ss.str();
    }

    KeyPressedEvent::KeyPressedEvent(unsigned int keyCode) : KeyEvent("KeyPressedEvent", EventType::KeyPressed, keyCode) {
    }

    KeyReleasedEvent::KeyReleasedEvent(unsigned int keyCode) : KeyEvent("KeyReleasedEvent", EventType::KeyReleased, keyCode) {
    }

    KeyRepeatedEvent::KeyRepeatedEvent(unsigned int keyCode) : KeyEvent("KeyRepeatedEvent", EventType::KeyRepeated, keyCode) {
    }

    KeyTypedEvent::KeyTypedEvent(unsigned int keyCode) : KeyEvent("KeyTypedEvent", EventType::KeyTyped, keyCode) {
    }

}
