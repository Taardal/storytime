#pragma once

#include "system/st_event.h"

namespace Storytime {
    struct ImGuiWindowResizeEvent : Event {
        static const EventType type;
        static const std::string name;

        std::string window_id;
        i32 width;
        i32 height;

        ImGuiWindowResizeEvent(const std::string& window_id, i32 width, i32 height);

        std::string to_string() const override;
    };
}