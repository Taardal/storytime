#pragma once

#include "system/st_event.h"

namespace Storytime {
    struct WindowCloseEvent : Event {
        static const EventType type;
        static const std::string name;
        WindowCloseEvent();
    };

    struct WindowResizeEvent : Event {
        static const EventType type;
        static const std::string name;
        i32 width;
        i32 height;

        WindowResizeEvent(i32 width, i32 height);

        std::string to_string() const override;
    };

    struct WindowMinimizeEvent final : Event {
        static const EventType type;
        static const std::string name;
        bool minimized;

        explicit WindowMinimizeEvent(bool minimized);

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowMinimizeEvent& event);
}
