#pragma once

#include "system/event.h"

namespace Storytime {
    struct MouseMovedEvent : Event {
        static const EventType type;
        static const std::string name;
        f32 x;
        f32 y;

        MouseMovedEvent(f32 x, f32 y);

        std::string to_string() const override;
    };

    struct MouseScrollEvent : Event {
        static const EventType type;
        static const std::string name;
        f32 x_offset;
        f32 y_offset;

        MouseScrollEvent(f32 x_offset, f32 y_offset);

        std::string to_string() const override;
    };

    struct MouseButtonEvent : Event {
        i32 button;

        MouseButtonEvent(EventType type, const std::string& name, i32 button);

        std::string to_string() const override;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent {
        static const EventType type;
        static const std::string name;
        explicit MouseButtonPressedEvent(i32 button);
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent {
        static const EventType type;
        static const std::string name;
        explicit MouseButtonReleasedEvent(i32 button);
    };
}
