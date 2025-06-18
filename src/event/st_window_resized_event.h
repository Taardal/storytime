#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct WindowResizedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 width = 0;
        i32 height = 0;

        WindowResizedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowResizedEvent& event);
}
