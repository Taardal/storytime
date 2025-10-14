#pragma once

#include "st_config.h"
#include "st_app.h"
#include "audio/st_audio_engine.h"
#include "graphics/st_imgui_renderer.h"
#include "graphics/st_renderer.h"
#include "process/st_process_manager.h"
#include "resource/st_resource_loader.h"
#include "system/st_dispatcher.h"
#include "system/st_file_reader.h"
#include "system/st_metrics.h"
#include "system/st_service_locator.h"
#include "window/st_keyboard.h"
#include "window/st_mouse.h"
#include "window/st_window.h"

namespace Storytime {
    class Engine {
        friend class Storytime;

    private:
        bool running = false;
        ServiceLocator service_locator;
        Dispatcher dispatcher;
        FileReader file_reader;
        AudioEngine audio_engine;
        ResourceLoader resource_loader;
        Window window;
#ifndef ST_USE_VULKAN
        ImGuiRenderer imgui_renderer;
#endif
        Keyboard keyboard;
        Mouse mouse;
        ProcessManager process_manager;
        Metrics metrics;
        Renderer vulkan_renderer;

    public:
        Engine(const Config& config);

        void run(App& app);

        void stop();

        template<typename T>
        T* get() const {
            return service_locator.get<T>();
        }

    private:
        void run_game_loop(App& app);
    };
}
