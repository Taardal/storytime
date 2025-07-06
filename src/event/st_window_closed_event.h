#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct WindowClosedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        WindowClosedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowClosedEvent& event);
}
