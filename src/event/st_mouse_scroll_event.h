#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct MouseScrollEvent : Event {
        static const EventType type;
        static const std::string type_name;

        f32 x_offset = 0.0f;
        f32 y_offset = 0.0f;

        MouseScrollEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const MouseScrollEvent& event);
}
