#include "user_input.h"

#include <GLFW/glfw3.h>

namespace Storytime {
    bool UserInput::is_key_pressed(KeyCode key_code) {
        return glfwGetKey(glfwGetCurrentContext(), key_code) == GLFW_PRESS;
    }

    bool UserInput::is_mouse_button_pressed(MouseButtonCode mouse_button_code) {
        return glfwGetMouseButton(glfwGetCurrentContext(), mouse_button_code) == GLFW_PRESS;
    }

    std::pair<f32, f32> UserInput::get_mouse_position() {
        double x;
        double y;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        return {(f32) x, (f32) y};
    }

    bool Keyboard::is_pressed(KeyCode key_code) {
        return UserInput::is_key_pressed(key_code);
    }

    bool Mouse::is_pressed(MouseButtonCode mouse_button_code) {
        return UserInput::is_mouse_button_pressed(mouse_button_code);
    }

    std::pair<f32, f32> Mouse::get_position() {
        return UserInput::get_mouse_position();
    }
}
