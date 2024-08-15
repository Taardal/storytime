#pragma once

#include "event.h"

namespace Storytime {
    struct MouseMovedEvent : Event {
        f32 x;
        f32 y;

        MouseMovedEvent(f32 x, f32 y);

        std::string to_string() const override;
    };

    struct MouseScrollEvent : Event {
        f32 x_offset;
        f32 y_offset;

        MouseScrollEvent(f32 xOffset, f32 yOffset);

        std::string to_string() const override;
    };

    struct MouseButtonEvent : Event {
        i32 button;

        MouseButtonEvent(EventType type, const std::string& name, i32 button);

        std::string to_string() const override;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent {
        explicit MouseButtonPressedEvent(i32 button);
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent {
        explicit MouseButtonReleasedEvent(i32 button);
    };
}
