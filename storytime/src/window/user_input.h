#pragma once

#include "key.h"
#include "mouse_button.h"

namespace Storytime {
    class UserInput {
    public:
        static bool is_key_pressed(KeyCode key_code);

        static bool is_mouse_button_pressed(MouseButtonCode mouse_button_code);

        static std::pair<f32, f32> get_mouse_position();
    };

    class Keyboard {
    public:
        static bool is_pressed(KeyCode key_code);
    };

    class Mouse {
    public:
        static bool is_pressed(MouseButtonCode mouse_button_code);

        static std::pair<f32, f32> get_position();
    };
}
