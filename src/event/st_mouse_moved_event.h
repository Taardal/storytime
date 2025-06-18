#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct MouseMovedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        f32 x = 0.0f;
        f32 y = 0.0f;

        MouseMovedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const MouseMovedEvent& event);
}
