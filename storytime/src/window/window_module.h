#pragma once

#include "storytime_config.h"
#include "system/system_module.h"
#include "window/window.h"
#include "window/event_manager.h"

namespace Storytime {
    struct WindowModule {
        Window window;
        EventManager event_manager;

        WindowModule(const Config& config, SystemModule* system_module);

        static void initialize();

        static void terminate();
    };
}
