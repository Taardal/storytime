#pragma once

#include "system/st_command_line_arguments.h"
#include "system/st_dispatcher.h"
#include "system/st_service_locator.h"

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
        Dispatcher* dispatcher = nullptr;

    public:
        Storytime(Config* config, ServiceLocator* service_locator, Dispatcher* dispatcher)
            : config(config), service_locator(service_locator), dispatcher(dispatcher) {
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

        template<typename T>
        SubscriptionID subscribe(const SubscriptionFn<T>& subscription_fn) const {
            ST_ASSERT(dispatcher != nullptr, "Dispatcher cannot be null");
            return dispatcher->subscribe<T>(subscription_fn);
        }

        void set_log_level(LogLevel log_level) const {
            ST_ASSERT(config != nullptr, "Config cannot be null");
            config->log_level = log_level;
            ::Storytime::set_log_level(log_level);
        }
    };
}