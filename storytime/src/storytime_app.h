#pragma once

#include "storytime_config.h"
#include "system/service_locator.h"
#include "window/event_manager.h"
#include "window/window.h"
#include "graphics/open_gl.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/camera.h"

namespace Storytime {
    class App {
    private:
        bool running = false;
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

        virtual void on_update(Camera* camera, f64 timestep) = 0;

        virtual void on_render(Renderer* renderer) = 0;

        virtual void on_imgui_render() {}

    private:
        void game_loop();
    };
}