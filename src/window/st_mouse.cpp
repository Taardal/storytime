#include "st_mouse.h"

namespace Storytime {
    Mouse::Mouse(const MouseConfig& config) : config(config) {}

    Mouse::Mouse(MouseConfig&& config) : config(std::move(config)) {}

    bool Mouse::is_enabled() const {
        return enabled;
    }

    void Mouse::set_enabled(bool enabled) {
        this->enabled = enabled;
    }

    bool Mouse::is_pressed(MouseButtonCode mouse_button_code) const {
        if (!enabled) {
            return false;
        }
        ST_ASSERT(config.window != nullptr, "Window must exist");
        return glfwGetMouseButton(*config.window, mouse_button_code) == GLFW_PRESS;
    }

    std::pair<f32, f32> Mouse::get_position() const {
        if (!enabled) {
            return {0.0f, 0.0f};
        }
        ST_ASSERT(config.window != nullptr, "Window must exist");
        double x;
        double y;
        glfwGetCursorPos(*config.window, &x, &y);
        return {(f32) x, (f32) y};
    }
}