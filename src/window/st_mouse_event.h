#pragma once

#include "system/st_event.h"

namespace Storytime {
    struct MouseMovedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        f32 x;
        f32 y;

        MouseMovedEvent(f32 x, f32 y);

        std::string to_string() const override;
    };

    struct MouseScrollEvent : Event {
        static const EventType type;
        static const std::string type_name;

        f32 x_offset;
        f32 y_offset;

        MouseScrollEvent(f32 x_offset, f32 y_offset);

        std::string to_string() const override;
    };

    struct MouseButtonEvent : Event {
        i32 button;

        MouseButtonEvent(EventType type, const std::string& type_name, i32 button);

        std::string to_string() const override;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent {
        static const EventType type;
        static const std::string type_name;

        explicit MouseButtonPressedEvent(i32 button);
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent {
        static const EventType type;
        static const std::string type_name;

        explicit MouseButtonReleasedEvent(i32 button);
    };
}
