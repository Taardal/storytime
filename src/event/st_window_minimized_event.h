#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct WindowMinimizedEvent final : Event {
        static const EventType type;
        static const std::string type_name;

        bool minimized = false;

        WindowMinimizedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowMinimizedEvent& event);
}
