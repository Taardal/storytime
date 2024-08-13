#pragma once

#include "window/Input.h"
#include "window/window.h"

namespace Storytime {
    struct WindowModuleConfig {
        WindowConfig window_config;
    };

    struct WindowModule {
        WindowModuleConfig config;
        EventManager event_manager;
        Window window;
        Input input;

        explicit WindowModule(const WindowModuleConfig& config);
    };
}
