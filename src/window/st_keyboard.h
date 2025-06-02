#pragma once

#include "st_key.h"
#include "st_window.h"

namespace Storytime {
    struct KeyboardConfig {
        Window* window = nullptr;
    };

    class Keyboard {
    private:
        KeyboardConfig config;
        bool enabled = true;

    public:
        Keyboard(const KeyboardConfig& config);

        Keyboard(KeyboardConfig&& config);

    public:
        bool is_enabled() const;

        void set_enabled(bool enabled);

        bool is_pressed(KeyCode key_code) const;
    };
}
