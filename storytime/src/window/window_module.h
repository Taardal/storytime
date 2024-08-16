#pragma once

#include "system/system_module.h"
#include "window/user_input.h"
#include "window/window.h"

namespace Storytime {
    struct WindowModuleConfig {
        SystemModule* system_module = nullptr;
        WindowConfig window_config;
    };

    struct WindowModule {
        WindowModuleConfig config;
        EventManager event_manager;
        Window window;
        UserInput user_input;

        explicit WindowModule(const WindowModuleConfig& config);
    };
}
