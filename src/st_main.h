#pragma once

#include "system/command_line_arguments.h"
#include "system/service_locator.h"
#include "system/event_manager.h"

namespace Storytime {
    struct Config {
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;
        u32 target_fps = 60;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 768;
        f32 window_aspect_ratio = 16.0f / 9.0f;
        bool window_maximized = false;
        bool window_resizable = true;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string glsl_version = "#version 410";
    };

    void run(const Config& config);

    void stop();
}

namespace Storytime {
    class Storytime {
    private:
        Config config;
        ServiceLocator* service_locator = nullptr;
        EventManager* event_manager = nullptr;

    public:
        Storytime(const Config& config, ServiceLocator* service_locator, EventManager* event_manager)
            : config(config), service_locator(service_locator), event_manager(event_manager) {
        }

        const Config& cfg() const {
            return config;
        }

        const CommandLineArguments& args() const {
            return config.command_line_arguments;
        }

        template<class T>
        T* get() const {
            ST_ASSERT(service_locator != nullptr, "Service locator cannot be null");
            return service_locator->get<T>();
        }

        SubscriptionID subscribe(EventType event_type, const Subscription& subscription) const {
            ST_ASSERT(event_manager != nullptr, "Event manager cannot be null");
            return event_manager->subscribe(event_type, subscription);
        }
    };
}