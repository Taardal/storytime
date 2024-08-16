#pragma once

#include "key.h"
#include "mouse_button.h"

namespace Storytime {
    class UserInput {
    public:
        bool is_key_pressed(KeyCode key_code) const;

        bool is_mouse_button_pressed(MouseButtonCode mouse_button_code) const;

        std::pair<float, float> get_mouse_position() const;
    };
}
