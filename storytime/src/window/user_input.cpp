#include "user_input.h"

#include <GLFW/glfw3.h>

namespace Storytime {
    bool UserInput::is_key_pressed(KeyCode key_code) const {
        return glfwGetKey(glfwGetCurrentContext(), key_code) == GLFW_PRESS;
    }

    bool UserInput::is_mouse_button_pressed(MouseButtonCode mouse_button_code) const {
        return glfwGetMouseButton(glfwGetCurrentContext(), mouse_button_code) == GLFW_PRESS;
    }

    std::pair<f32, f32> UserInput::get_mouse_position() const {
        double x;
        double y;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        return {(f32) x, (f32) y};
    }
}
