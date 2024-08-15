#pragma once

#include "event.h"

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
        KeyPressedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyReleasedEvent : KeyEvent {
        KeyReleasedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyRepeatedEvent : KeyEvent {
        KeyRepeatedEvent(i32 key_code, i32 mods, i32 scan_code);
    };

    struct KeyTypedEvent : KeyEvent {
        KeyTypedEvent(i32 key_code, i32 mods, i32 scan_code);
    };
}
