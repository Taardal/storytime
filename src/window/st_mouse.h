#pragma once

#include "st_mouse_button.h"
#include "st_window.h"

namespace Storytime {
    struct MouseConfig {
        const Window& window;
    };

    class Mouse {
    private:
        MouseConfig config;
        bool enabled = true;

    public:
        Mouse(const MouseConfig& config);

        Mouse(MouseConfig&& config);

        bool is_enabled() const;

        void set_enabled(bool enabled);

        bool is_pressed(MouseButtonCode mouse_button_code) const;

        std::pair<f32, f32> get_position() const;
    };
}
