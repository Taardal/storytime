#pragma once

#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"
#include "service_locator.h"

namespace Storytime {
    struct AppConfig {
        LogLevel log_level;
        std::string window_title;
        int32_t window_width;
        int32_t window_height;
        bool window_maximized;
        bool window_resizable;
        uint32_t open_gl_version_major;
        uint32_t open_gl_version_minor;
        const char* glsl_version;
    };

    class Application {
    private:
        bool running = false;
        SystemModule system_module;
        WindowModule window_module;
        GraphicsModule graphics_module;

    protected:
        ServiceLocator service_locator;

    public:
        explicit Application(const AppConfig& config);

        virtual ~Application() = default;

        void run();

        void stop();

    protected:
        virtual void on_update(f64 timestep) = 0;

        virtual void on_render() = 0;
    };
}
