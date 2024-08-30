#pragma once

#include "storytime_config.h"
#include "window/window.h"
#include "window/event_manager.h"

#include "system/module.h"

namespace Storytime {
    class WindowModule : public Module {
    private:
        EventManager event_manager;
        Window window;

    public:
        explicit WindowModule(const Config& config);

        static void initialize();

        static void terminate();
    };
}
