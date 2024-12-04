#pragma once

#include "system/event.h"

#define CREATE_RANDOM_EVENT_TYPE(a) a
#define EVENT_TYPE() CREATE_RANDOM_EVENT_TYPE(__COUNTER__)

namespace Storytime {
    struct KeyEvent : Event {
        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyEvent(EventType type, const std::string& name, i32 key_code);

        KeyEvent(EventType type, const std::string& name, i32 key_code, i32 mods, i32 scan_code);

        std::string to_string() const override;
    };

    struct KeyPressedEvent : KeyEvent {
        static const EventType type;
        static const std::string name;
        KeyPressedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyReleasedEvent : KeyEvent {
        static const EventType type;
        static const std::string name;
        KeyReleasedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyRepeatedEvent : KeyEvent {
        static const EventType type;
        static const std::string name;
        KeyRepeatedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyTypedEvent : KeyEvent {
        static const EventType type;
        static const std::string name;
        KeyTypedEvent(i32 key_code, i32 mods, i32 scan_code);
    };
}
