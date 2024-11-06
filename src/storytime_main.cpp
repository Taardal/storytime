#include "storytime_main.h"
#include "system/service_locator.h"
#include "system/file_system.h"
#include "system/event_manager.h"
#include "window/window.h"
#include "audio/audio_engine.h"
#include "resource/resource_loader.h"
#include "graphics/open_gl.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "system/clock.h"
#include "window/mouse_event.h"
#include "window/window_event.h"

extern "C" void on_create();

extern "C" void on_update(f64 timestep);

extern "C" void on_render();

extern "C" void on_imgui_render();

extern "C" void on_destroy();

namespace Storytime {
    static ServiceLocator* service_locator_ptr = nullptr;
    static EventManager* event_manager_ptr = nullptr;
    static bool running = false;

    void run(const Config& config) {
        initialize_error_signal_handlers();
        set_log_level(config.log_level);

#ifdef ST_TRACK_MEMORY
        initialize_memory_tracking();
        std::atexit(terminate_memory_tracking);
#endif

        try {
            ST_LOG_INFO("Initializing...");

            EventManager event_manager({
                .queue_count = 1,
            });
            event_manager.subscribe(WindowCloseEvent::type, [&](const Event&) {
                stop();
            });
            event_manager_ptr = &event_manager; // Application lifetime is encapsulated in this scope

            Window window({
                .event_manager = &event_manager,
                .title = config.window_title,
                .width = config.window_width,
                .height = config.window_height,
                .maximized = config.window_maximized,
                .resizable = config.window_resizable,
                .context_version_major = config.open_gl_version_major,
                .context_version_minor = config.open_gl_version_minor,
            });

            OpenGL::initialize({
                .window = &window,
                .log_level = config.log_level,
                .major_version = config.open_gl_version_major,
                .minor_version = config.open_gl_version_minor,
                .glsl_version = config.glsl_version,
            });

            FileSystem file_system;

            AudioEngine audio_engine;

            ResourceLoader resource_loader({
                .file_system = &file_system,
                .audio_engine = &audio_engine
            });

            WindowSize window_size_px = window.get_size_in_pixels();
            Renderer renderer({
                .resource_loader = &resource_loader,
                .viewport = {
                    .width = window_size_px.width,
                    .height = window_size_px.height,
                },
            });
            event_manager.subscribe(WindowResizeEvent::type, [&renderer](const Event& event) {
                auto& window_resize_event = (WindowResizeEvent&) event;
                renderer.set_viewport({
                    .width = window_resize_event.width,
                    .height = window_resize_event.height,
                });
            });

            ImGuiRenderer imgui_renderer({
                .window = &window,
                .glsl_version = config.glsl_version,
            });

            ServiceLocator service_locator;
            service_locator.set<EventManager>(&event_manager);
            service_locator.set<Window>(&window);
            service_locator.set<FileSystem>(&file_system);
            service_locator.set<AudioEngine>(&audio_engine);
            service_locator.set<ResourceLoader>(&resource_loader);
            service_locator.set<Renderer>(&renderer);
            service_locator.set<ImGuiRenderer>(&imgui_renderer);
            service_locator_ptr = &service_locator; // Application lifetime is encapsulated in this scope

            on_create();
            ST_LOG_INFO("Created client");

            running = true;
            ST_LOG_INFO("Running...");

            Clock clock;
            clock.start();

            f64 last_uptime_sec = 0.0;

            while (running) {
                window.update();

                f64 uptime_sec = clock.get_time<ms>() / 1000;
                f64 timestep = std::min(uptime_sec - last_uptime_sec, 1.0);
                last_uptime_sec = uptime_sec;

                on_update(timestep);
                event_manager.process_event_queue();

                renderer.begin_frame();
                on_render();
                renderer.end_frame();

#ifdef ST_DEBUG
                imgui_renderer.begin_frame();
                on_imgui_render();
                imgui_renderer.end_frame();
#endif
            }

            ST_LOG_INFO("Terminating...");

            on_destroy();
            ST_LOG_INFO("Destroyed client");

        } catch (const Error& e) {
            ST_LOG_CRITICAL("Fatal error");
            e.print_stacktrace();
        } catch (const std::exception& e) {
            ST_LOG_CRITICAL("Fatal error: {}", e.what());
        }

    }

    void stop() {
        running = false;
    }

    void* get_service(std::type_index type) {
        ST_ASSERT(service_locator_ptr != nullptr, "Invalid service locator pointer");
        auto service = service_locator_ptr->get(type);
        ST_ASSERT(service != nullptr, "Service must exist");
        return service;
    }

    u32 subscribe(const EventType event_type, const Subscription& subscription) {
        ST_ASSERT(event_manager_ptr != nullptr, "Invalid event manager pointer");
        return event_manager_ptr->subscribe(event_type, subscription);
    }
}
