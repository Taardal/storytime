#include "storytime_main.h"
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
#include "system/clock.h"

extern "C" void on_create(const Storytime::Storytime& storytime);

extern "C" void on_update(f64 timestep);

extern "C" void on_render();

extern "C" void on_destroy();

namespace Storytime {
    bool running = false;

    void start(const Config& config) {
#ifdef ST_TRACK_MEMORY
        std::atexit(MemoryTracker::terminate);
#endif
        initialize_error_signal_handlers();
        set_log_level(config.log_level);
        try {
            running = true;
            run(config);
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

    void run(const Config& config) {
        // -------------------------------------------------------------------------------------
        // Initialize engine
        // -------------------------------------------------------------------------------------

        FileSystem file_system;

        EventManager event_manager({
            .queue_count = 1,
        });
        event_manager.subscribe(EventType::WindowClose, [&](const Event& event) {
            stop();
        });

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

        AudioEngine audio_engine;

        ResourceLoader resource_loader({
            .file_system = &file_system,
            .audio_engine = &audio_engine
        });

        OpenGL open_gl({
            .window = &window,
            .log_level = config.log_level,
            .major_version = config.open_gl_version_major,
            .minor_version = config.open_gl_version_minor,
            .glsl_version = config.glsl_version,
        });

        Renderer renderer(&resource_loader);
        renderer.set_clear_color({0.1f, 0.1f, 0.1f});

        ImGuiRenderer imgui_renderer({
            .window = &window,
            .glsl_version = config.glsl_version,
        });

        Camera camera;
        camera.set_projection({
            .left = 0,
            .right = static_cast<float>(config.window_width),
            .top = 0,
            .bottom = static_cast<float>(config.window_height),
        });

        ServiceLocator service_locator;
        service_locator.set<Window>(&window);
        service_locator.set<EventManager>(&event_manager);
        service_locator.set<AudioEngine>(&audio_engine);
        service_locator.set<ResourceLoader>(&resource_loader);
        service_locator.set<Renderer>(&renderer);
        service_locator.set<ImGuiRenderer>(&imgui_renderer);
        service_locator.set<Camera>(&camera);

        // -------------------------------------------------------------------------------------
        // Prepare game loop
        // -------------------------------------------------------------------------------------

        auto game_loop = [&] {
            struct Statistics {
                u32 ups = 0;
                u32 fps = 0;
            } statistics;

            f64 last_uptime_sec = 0.0;
            f64 frame_statistics_timestep = 0.0;

            Clock clock;
            clock.start();

            while (running) {
                window.update();

                f64 uptime_sec = clock.get_time<ms>() / 1000;
                f64 timestep = std::min(uptime_sec - last_uptime_sec, 1.0);
                last_uptime_sec = uptime_sec;

                on_update(timestep);
                event_manager.process_event_queue();
                statistics.ups++;

                renderer.begin_frame(camera.get_view_projection());
                on_render();
                renderer.end_frame();
                statistics.fps++;

#ifdef ST_DEBUG
                imgui_renderer.begin_frame();
                // on_imgui_render();
                imgui_renderer.end_frame();
#endif

                frame_statistics_timestep += timestep;
                if (frame_statistics_timestep >= 1.0) {
                    frame_statistics_timestep = 0;
                    // on_statistics(statistics);
                    statistics.ups = 0;
                    statistics.fps = 0;
                }
            }
        };

        // -------------------------------------------------------------------------------------
        // Run game
        // -------------------------------------------------------------------------------------

        Shared<Error> error = nullptr;

        // Create the client
        try {
            on_create(Storytime(config, &service_locator));
        } catch (const Error& e) {
            error = std::make_shared<Error>("Client creation error", ST_TAG, std::make_shared<Error>(e));
        } catch (const std::exception& e) {
            error = std::make_shared<Error>("Client creation error", ST_TAG, std::make_shared<Error>(e.what()));
        }

        // Run the game loop if the client was created without errors
        if (error == nullptr) {
            try {
                game_loop();
            } catch (const Error& e) {
                error = std::make_shared<Error>("Game loop error", ST_TAG, std::make_shared<Error>(e));
            } catch (const std::exception& e) {
                error = std::make_shared<Error>("Game loop error", ST_TAG, std::make_shared<Error>(e.what()));
            }
        }

        // Destroy the client, even if an error occurred during client creation or game loop, to allow the client
        // to do cleanup, like closing file or network connections or flushing logs or save game states, before the
        // program exits.
        on_destroy();

        // If any error occurred, throw the error up the chain after the client is destroyed
        if (error != nullptr) {
            throw *error;
        }
    }
}
