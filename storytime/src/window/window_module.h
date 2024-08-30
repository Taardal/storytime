#pragma once

#include "storytime_config.h"
#include "system/system_module.h"
#include "window/window.h"
#include "window/event_manager.h"

#include "system/module.h"

namespace Storytime {
    struct WindowModule : Module {
        SystemModule* system_module;
        EventManager event_manager;
        Window window;

        WindowModule(const Config& config, SystemModule* system_module);

        static void initialize();

        static void terminate();

        template<typename T>
        T* get() {
            return system_module->service_locator.get<T>();
        }
    };
}
