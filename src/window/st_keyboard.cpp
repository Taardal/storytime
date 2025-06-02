#include "st_keyboard.h"

namespace Storytime {
    Keyboard::Keyboard(const KeyboardConfig& config) : config(config) {}

    Keyboard::Keyboard(KeyboardConfig&& config) : config(std::move(config)) {}

    bool Keyboard::is_enabled() const {
        return enabled;
    }

    void Keyboard::set_enabled(bool enabled) {
        this->enabled = enabled;
    }

    bool Keyboard::is_pressed(KeyCode key_code) const {
        if (!enabled) {
            ST_LOG_W("NOTIC-----------------------------E");
            return false;
        }
        ST_ASSERT(config.window != nullptr, "Window must exist");
        return glfwGetKey(*config.window, key_code) == GLFW_PRESS;
    }
}
