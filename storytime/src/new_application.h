#pragma once

#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"

namespace Storytime {
    struct NewApplicationConfig {
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

    class NewApplication {
    private:
        SystemModule system_module;
        WindowModule window_module;
        GraphicsModule graphics_module;
        bool running;

    protected:
        EventManager* event_manager;
        NewWindow* window;
        Renderer* renderer;
        OrthographicCamera* camera;
        Input* input;

    public:
        explicit NewApplication(const NewApplicationConfig& config);

        virtual ~NewApplication() = default;

        void run();

        void stop();

    protected:
        virtual void on_update(f64 timestep) = 0;

        virtual void on_render() = 0;
    };
}
