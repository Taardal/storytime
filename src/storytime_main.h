#pragma once

#include "system/command_line_arguments.h"
#include "system/service_locator.h"
#include "window/event_manager.h"

namespace Storytime {
    struct Config {
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;
        u32 target_fps = 60;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 760;
        bool window_maximized = false;
        bool window_resizable = true;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string glsl_version = "#version 410";
    };

    class Storytime {
    private:
        Config config;
        ServiceLocator* service_locator = nullptr;

    public:
        Storytime(const Config& config, ServiceLocator* service_locator)
            : config(config), service_locator(service_locator) {
        }

        const Config& cfg() const {
            return config;
        }

        const CommandLineArguments& args() const {
            return config.command_line_arguments;
        }

        template<class T>
        T* get() const {
            ST_ASSERT(service_locator != nullptr);
            return service_locator->get<T>();
        }
    };

    void start(const Config& config);

    void stop();
}
