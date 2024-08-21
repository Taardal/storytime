#pragma once

#include "storytime_config.h"
#include "system/system_module.h"
#include "window/user_input.h"
#include "window/window.h"

namespace Storytime {
    struct WindowModule {
        EventManager event_manager;
        Window window;
        UserInput user_input;

        WindowModule(const Config& config, SystemModule* system_module);
    };
}
