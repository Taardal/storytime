#pragma once

#include "window/Input.h"
#include "window/Window.h"
#include "window/new_window.h"

namespace Storytime {
    struct WindowModuleConfig {
        NewWindowConfig window_config;
    };

    struct WindowModule {
        WindowModuleConfig config;
        EventManager event_manager;
        NewWindow window;
        Input input;

        explicit WindowModule(const WindowModuleConfig& config);
    };
}
