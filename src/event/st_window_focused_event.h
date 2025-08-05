#pragma once

#include "event/st_event.h"

namespace Storytime {
    struct WindowFocusedEvent final : Event {
        static const EventType type;
        static const std::string type_name;

        bool focused = false;

        WindowFocusedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowFocusedEvent& event);
}
