#include "st_run.h"

#include "audio/st_audio_engine.h"
#include "event/st_window_closed_event.h"
#include "event/st_window_resized_event.h"
#include "graphics/st_open_gl.h"
#include "graphics/st_renderer.h"
#include "process/st_process_manager.h"
#include "resource/st_resource_loader.h"
#include "system/st_clock.h"
#include "system/st_dispatcher.h"
#include "system/st_file_reader.h"
#include "system/st_game_loop_metrics.h"
#include "system/st_service_locator.h"
#include "window/st_keyboard.h"
#include "window/st_mouse.h"
#include "window/st_window.h"

#ifdef ST_IMGUI_ENABLED
    #include "graphics/st_imgui_renderer.h"
#endif

extern "C" void on_create(const Storytime::Storytime&);

extern "C" void on_update(f64 timestep);

extern "C" void on_render();

extern "C" void on_destroy();

#ifdef ST_IMGUI_ENABLED
    extern "C" void on_render_imgui();
#endif

namespace Storytime {
    static bool running = false;

    void run(const Config& config) {
        initialize_log(config.log_level);
        initialize_error_signal_handlers();
#ifdef ST_TRACK_MEMORY
        initialize_memory_tracking();
        std::atexit(terminate_memory_tracking);
#endif

        try {
            ST_LOG_INFO("Initializing...");
            ServiceLocator service_locator;

            Dispatcher dispatcher{};
            service_locator.set<Dispatcher>(&dispatcher);

            std::vector<SubscriptionID> event_subscriptions;

            event_subscriptions.push_back(
                dispatcher.subscribe<WindowClosedEvent>([&](const WindowClosedEvent&) {
                    stop();
                })
            );

            Window window({
                .title = config.window_title,
                .width = config.window_width,
                .height = config.window_height,
                .aspect_ratio = config.window_aspect_ratio,
                .maximized = config.window_maximized,
                .resizable = config.window_resizable,
                .context_version_major = config.open_gl_version_major,
                .context_version_minor = config.open_gl_version_minor,
                .dispatcher = &dispatcher,
            });
            service_locator.set<Window>(&window);

            Keyboard keyboard({
                .window = &window,
                .dispatcher = &dispatcher,
            });
            service_locator.set<Keyboard>(&keyboard);

            Mouse mouse({
                .window = &window,
            });
            service_locator.set<Mouse>(&mouse);

            OpenGL::initialize({
                .window = &window,
                .log_level = config.log_level,
                .major_version = config.open_gl_version_major,
                .minor_version = config.open_gl_version_minor,
                .glsl_version = config.glsl_version,
            });

            FileReader file_reader;
            service_locator.set<FileReader>(&file_reader);

            AudioEngine audio_engine;
            service_locator.set<AudioEngine>(&audio_engine);

            ResourceLoader resource_loader({
                .file_reader = &file_reader,
                .audio_engine = &audio_engine
            });
            service_locator.set<ResourceLoader>(&resource_loader);

            WindowSize window_size_px = window.get_size_in_pixels();
            Renderer renderer({
                .resource_loader = &resource_loader,
                .viewport = {
                    .width = window_size_px.width,
                    .height = window_size_px.height,
                },
            });
            event_subscriptions.push_back(
                dispatcher.subscribe<WindowResizedEvent>([&renderer](const WindowResizedEvent& event) {
                    renderer.set_viewport({
                        .width = event.width,
                        .height = event.height,
                    });
                })
            );
            service_locator.set<Renderer>(&renderer);

#ifdef ST_IMGUI_ENABLED
            ImGuiRenderer imgui_renderer({
                .settings_file_path = config.imgui_settings_file_path,
                .glsl_version = config.glsl_version,
                .window = &window,
                .keyboard = &keyboard,
                .mouse = &mouse,
            });
            service_locator.set<ImGuiRenderer>(&imgui_renderer);
#endif

            ProcessManager process_manager{};
            service_locator.set<ProcessManager>(&process_manager);

            GameLoopMetrics metrics{};
            service_locator.set<GameLoopMetrics>(&metrics);

            Storytime storytime(
                &const_cast<Config&>(config),
                &service_locator,
                &dispatcher
            );

            on_create(storytime);
            ST_LOG_INFO("Created client");

            //
            // GAME LOOP
            //

            // Update game at fixed timesteps to have game systems update at a predictable rate
            constexpr f64 timestep_sec = 1.0 / 60.0;
            constexpr f64 timestep_ms = timestep_sec * 1000.0;

            // How far the game clock is behind the app clock
            f64 game_clock_lag_ms = 0.0;

            // Use the duration of the last game loop cycle to increment game clock lag
            TimePoint last_cycle_start_time = Time::now();

            running = true;
            ST_LOG_INFO("Running...");

            while (running) {
                TimePoint cycle_start_time = Time::now();
                f64 last_cycle_duration_ms = Time::as<Microseconds>(cycle_start_time - last_cycle_start_time).count() / 1000.0;

                // If the last cycle lasted too long, assume that we have resumed from a breakpoint or similar
                // and override the duration to the target timestep to avoid big spikes in game systems.
                if (last_cycle_duration_ms > 1000.0) {
                    last_cycle_duration_ms = timestep_ms;
                }

                last_cycle_start_time = cycle_start_time;
                game_clock_lag_ms += last_cycle_duration_ms;

                //
                // PROCESS INPUT
                //

                TimePoint window_event_start_time = Time::now();
                window.poll_events();
                TimePoint window_event_end_time = Time::now();

                //
                // UPDATE
                //

                i32 update_count = 0;
                f64 update_start_lag_ms = game_clock_lag_ms;
                TimePoint update_start_time = Time::now();

                // Update game clock at fixed timesteps to have game systems update at a predictable rate.
                // Whenever the game clock lags behind the app clock by one-or-more timesteps, tick the game
                // clock forwards until it's caught up.
                while (game_clock_lag_ms >= timestep_ms) {
                    on_update(timestep_sec);
                    game_clock_lag_ms -= timestep_ms;
                    update_count++;
                }

                TimePoint update_end_time = Time::now();
                f64 update_end_lag_ms = game_clock_lag_ms;

                //
                // RENDER
                //

                TimePoint render_start_time = Time::now();
                renderer.begin_frame();
                on_render();
                renderer.end_frame();
                TimePoint render_end_time = Time::now();

#ifdef ST_IMGUI_ENABLED
                TimePoint imgui_render_start_time = Time::now();
                imgui_renderer.begin_frame();
                on_render_imgui();
                imgui_renderer.end_frame();
                TimePoint imgui_render_end_time = Time::now();
#endif

                TimePoint swap_buffers_start_time = Time::now();
                window.swap_buffers();
                TimePoint swap_buffers_end_time = Time::now();

                TimePoint cycle_end_time = Time::now();

                //
                // METRICS
                //

                if (update_count > 0) {
                    metrics.update_timestep_ms = update_start_lag_ms - update_end_lag_ms;
                    metrics.updates_per_second = update_count / (metrics.update_timestep_ms / 1000.0);
                    metrics.update_duration_ms = Time::as<Microseconds>(update_end_time - update_start_time).count() / 1000.0;
                }
                metrics.render_duration_ms = Time::as<Microseconds>(render_end_time - render_start_time).count() / 1000.0;
                metrics.frames_per_second = 1.0 / (metrics.render_duration_ms / 1000.0);
#ifdef ST_IMGUI_ENABLED
                metrics.imgui_render_duration_ms = Time::as<Microseconds>(imgui_render_end_time - imgui_render_start_time).count() / 1000.0;
#endif
                metrics.window_events_duration_ms = Time::as<Microseconds>(window_event_end_time - window_event_start_time).count() / 1000.0;
                metrics.swap_buffers_duration_ms = Time::as<Microseconds>(swap_buffers_end_time - swap_buffers_start_time).count() / 1000.0;
                metrics.cycle_duration_ms = Time::as<Microseconds>(cycle_end_time - cycle_start_time).count() / 1000.0;
            }

            ST_LOG_INFO("Terminating...");

            on_destroy();
            ST_LOG_INFO("Destroyed client");

            dispatcher.unsubscribe_and_clear(event_subscriptions);
            ST_LOG_DEBUG("Unsubscribed event listeners");

        } catch (const Error& e) {
            ST_LOG_CRITICAL("Fatal error");
            e.print_stacktrace();
        } catch (const std::exception& e) {
            ST_LOG_CRITICAL("Fatal error [{}]: {}", ST_TYPE_NAME(e), e.what());
        }
    }

    void stop() {
        running = false;
    }
}
