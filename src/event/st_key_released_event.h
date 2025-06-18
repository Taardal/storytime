#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct KeyReleasedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyReleasedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyReleasedEvent& event);
}