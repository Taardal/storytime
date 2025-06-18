#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct MouseButtonReleasedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 button = 0;

        MouseButtonReleasedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const MouseButtonReleasedEvent& event);
}
