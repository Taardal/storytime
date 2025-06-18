#pragma once

#include "system/st_command_line_arguments.h"
#include "system/st_service_locator.h"
#include "event/st_event_manager.h"

namespace Storytime {
    struct Config {
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 768;
        f32 window_aspect_ratio = 16.0f / 9.0f;
        bool window_maximized = false;
        bool window_resizable = true;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string glsl_version = "#version 410";
        std::filesystem::path imgui_settings_file_path;
    };

    void run(const Config& config);

    void stop();
}

namespace Storytime {
    class Storytime {
    private:
        Config* config = nullptr;
        ServiceLocator* service_locator = nullptr;
        EventManager* event_manager = nullptr;

    public:
        Storytime(Config* config, ServiceLocator* service_locator, EventManager* event_manager)
            : config(config), service_locator(service_locator), event_manager(event_manager) {
        }

        const Config& cfg() const {
            ST_ASSERT(config != nullptr, "Config cannot be null");
            return *config;
        }

        const CommandLineArguments& args() const {
            ST_ASSERT(config != nullptr, "Config cannot be null");
            return config->command_line_arguments;
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

        void set_log_level(LogLevel log_level) const {
            ST_ASSERT(config != nullptr, "Config cannot be null");
            config->log_level = log_level;
            ::Storytime::set_log_level(log_level);
        }
    };
}