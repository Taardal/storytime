#pragma once

#include "st_config.h"
#include "st_app.h"
#include "audio/st_audio_engine.h"
#include "graphics/st_imgui_renderer.h"
#include "graphics/st_renderer.h"
#include "graphics/st_vulkan_context.h"
#include "graphics/st_vulkan_device.h"
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
        Window window;
        Keyboard keyboard;
        Mouse mouse;
        FileReader file_reader;
        Metrics metrics;
        VulkanContext vulkan_context;
        VulkanPhysicalDevice vulkan_physical_device;
        VulkanDevice vulkan_device;
        Renderer renderer;
#ifndef ST_USE_VULKAN
        ImGuiRenderer imgui_renderer;
#endif
        AudioEngine audio_engine;
        ResourceLoader resource_loader;
        ProcessManager process_manager;

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
