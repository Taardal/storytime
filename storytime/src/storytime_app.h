#pragma once

#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"

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

    class StorytimeApp {
    private:
        bool running = false;
        SystemModule system_module;
        WindowModule window_module;
        GraphicsModule graphics_module;

    public:
        explicit StorytimeApp(const AppConfig& config);

        virtual ~StorytimeApp() = default;

        void run();

        void stop();

    protected:
        template<typename T>
        T* get() {
            return system_module.service_locator.get<T>();
        }

        virtual void on_update(f64 timestep) = 0;

        virtual void on_render() = 0;
    };
}
