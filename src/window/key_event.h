#pragma once

#include "system/event.h"

namespace Storytime {
    struct KeyEvent : Event {
        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyEvent(EventType event_type, const std::string& event_type_name);

        KeyEvent(EventType event_type, const std::string& event_type_name, i32 key_code);

        KeyEvent(EventType event_type, const std::string& event_type_name, i32 key_code, i32 mods, i32 scan_code);

        std::string to_string() const override;
    };

    struct KeyPressedEvent : KeyEvent {
        static const EventType type;
        static const std::string type_name;

        KeyPressedEvent();

        KeyPressedEvent(i32 key_code, i32 mods, i32 scan_code);

        MSGPACK_DEFINE_MAP(event_type, event_type_name, key_code, mods, scan_code);
    };

    struct KeyReleasedEvent : KeyEvent {
        static const EventType type;
        static const std::string type_name;

        KeyReleasedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyRepeatedEvent : KeyEvent {
        static const EventType type;
        static const std::string type_name;

        KeyRepeatedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyTypedEvent : KeyEvent {
        static const EventType type;
        static const std::string type_name;

        KeyTypedEvent(i32 key_code, i32 mods, i32 scan_code);
    };
}
