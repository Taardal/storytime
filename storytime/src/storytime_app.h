#pragma once

#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"

namespace Storytime {
    struct AppConfig {
        LogLevel log_level = LogLevel::info;
        u32 target_fps = 60;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 760;
        bool window_maximized = false;
        bool window_resizable = true;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        const char* glsl_version = "#version 410";
    };

    class StorytimeApp {
    protected:
        AppConfig config;

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

        virtual void on_imgui_render() {}
    };
}
