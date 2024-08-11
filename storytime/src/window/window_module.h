#pragma once

#include "window/Window.h"
#include "window/new_window.h"

namespace Storytime {
    struct WindowModuleConfig {
        NewWindowConfig window_config;
    };

    struct WindowModule {
        WindowModuleConfig config;
        NewWindow window;

        explicit WindowModule(const WindowModuleConfig& config);
    };
}
