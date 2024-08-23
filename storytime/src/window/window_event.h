#pragma once

#include "event.h"

namespace Storytime {
    struct WindowCloseEvent : Event {
        WindowCloseEvent();
    };

    struct WindowResizeEvent : Event {
        i32 width;
        i32 height;

        WindowResizeEvent(i32 width, i32 height);

        std::string to_string() const override;
    };

    struct WindowMinimizeEvent final : Event {
        bool minimized;

        explicit WindowMinimizeEvent(bool minimized);

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowMinimizeEvent& event);
}
