#include "KeyEvent.h"
#include <sstream>

namespace storytime
{
    KeyEvent::KeyEvent(std::string_view name, EventType type, uint32_t keyCode)
            : Event(name, type), keyCode(keyCode)
    {
    }

    uint32_t KeyEvent::GetKeyCode() const
    {
        return keyCode;
    }

    std::string KeyEvent::ToString() const
    {
        std::stringstream ss;
        ss << name << "{keyCode=" << keyCode << "}";
        return ss.str();
    }

    KeyPressedEvent::KeyPressedEvent(uint32_t keyCode)
            : KeyEvent("KeyPressedEvent", EventType::KeyPressed, keyCode)
    {
    }

    KeyReleasedEvent::KeyReleasedEvent(uint32_t keyCode)
            : KeyEvent("KeyReleasedEvent", EventType::KeyReleased, keyCode)
    {
    }

    KeyRepeatedEvent::KeyRepeatedEvent(uint32_t keyCode)
            : KeyEvent("KeyRepeatedEvent", EventType::KeyRepeated, keyCode)
    {
    }

    KeyTypedEvent::KeyTypedEvent(uint32_t keyCode)
            : KeyEvent("KeyTypedEvent", EventType::KeyTyped, keyCode)
    {
    }

}
