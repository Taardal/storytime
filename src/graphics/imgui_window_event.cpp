#include "imgui_window_event.h"

namespace Storytime {
    const EventType ImGuiWindowResizeEvent::type = 4125;
    const std::string ImGuiWindowResizeEvent::name = "ImGuiWindowResizeEvent";

    ImGuiWindowResizeEvent::ImGuiWindowResizeEvent(const std::string& window_id, i32 width, i32 height)
        : Event(type, name), window_id(window_id), width(width), height(height) {
    }

    std::string ImGuiWindowResizeEvent::to_string() const {
        std::stringstream ss;
        ss << name << "{";
        ss << "window_id=" << window_id;
        ss << ", ";
        ss << "width=" << width;
        ss << ", ";
        ss << "height=" << height;
        ss << "}";
        return ss.str();
    }
}