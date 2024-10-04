#pragma once

#include "storytime_config.h"
#include "system/service_locator.h"
#include "system/file_system.h"
#include "window/event_manager.h"
#include "window/window.h"
#include "audio/audio_engine.h"
#include "resource/resource_loader.h"
#include "graphics/open_gl.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/camera.h"

namespace Storytime {
    class App {
    protected:
        struct Statistics {
            u32 ups = 0;
            u32 fps = 0;
        };

    protected:
        Config config;
        ServiceLocator service_locator;
        FileSystem file_system;
        EventManager event_manager;
        Window window;
        AudioEngine audio_engine;
        ResourceLoader resource_loader;
        OpenGL open_gl;
        Renderer renderer;
        ImGuiRenderer imgui_renderer;
        Camera camera;

    private:
        bool running = false;
        Statistics statistics{};

    public:
        explicit App(const Config& config);

        virtual ~App() = default;

        const Config& get_config() const;

        const CommandLineArguments& get_args() const;

        void run();

        void stop();

        template<typename T>
        T* get() {
            return service_locator.get<T>();
        }

    protected:
        virtual void on_initialize() {}

        virtual void on_terminate() {}

        virtual void on_update(f64 timestep) = 0;

        virtual void on_render() = 0;

        virtual void on_imgui_render() {}

        virtual void on_statistics(const Statistics& frame_statistics) {}

    private:
        void game_loop();
    };
}