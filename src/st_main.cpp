#include "st_main.h"
#include "audio/audio_engine.h"
#include "resource/resource_loader.h"
#include "graphics/open_gl.h"
#include "graphics/renderer.h"
#include "graphics/imgui_renderer.h"
#include "graphics/imgui_window_event.h"
#include "system/clock.h"
#include "system/event_manager.h"
#include "system/file_reader.h"
#include "system/game_loop_statistics.h"
#include "system/service_locator.h"
#include "window/window.h"
#include "window/window_event.h"

#ifdef ST_DEBUG
    #define ST_RENDER_IMGUI
#endif

extern "C" void on_create(const Storytime::Storytime&);

extern "C" void on_update(f64 timestep);

extern "C" void on_render();

extern "C" void on_render_imgui();

extern "C" void on_destroy();

namespace Storytime {
    static bool running = false;

    void run(const Config& config) {
        set_log_level(config.log_level);

        initialize_error_signal_handlers();

#ifdef ST_TRACK_MEMORY
        initialize_memory_tracking();
        std::atexit(terminate_memory_tracking);
#endif

        try {
            ST_LOG_INFO("Initializing...");

            EventManager event_manager({
                .queue_count = 1,
            });

            std::vector<SubscriptionID> event_subscriptions;
            event_subscriptions.push_back(
                event_manager.subscribe(WindowCloseEvent::type, [&](const Event&) {
                    stop();
                })
            );

            Window window({
                .event_manager = &event_manager,
                .title = config.window_title,
                .width = config.window_width,
                .height = config.window_height,
                .aspect_ratio = config.window_aspect_ratio,
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

            FileReader file_reader;

            AudioEngine audio_engine;

            ResourceLoader resource_loader({
                .file_reader = &file_reader,
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
            event_subscriptions.push_back(
                event_manager.subscribe(WindowResizeEvent::type, [&renderer](const Event& event) {
                    auto& window_resize_event = (WindowResizeEvent&) event;
                    renderer.set_viewport({
                        .width = window_resize_event.width,
                        .height = window_resize_event.height,
                    });
                })
            );

            ImGuiRenderer imgui_renderer({
                .glsl_version = config.glsl_version,
                .window = &window,
                .event_manager = &event_manager,
            });

            ServiceLocator service_locator;
            service_locator.set<EventManager>(&event_manager);
            service_locator.set<Window>(&window);
            service_locator.set<FileReader>(&file_reader);
            service_locator.set<AudioEngine>(&audio_engine);
            service_locator.set<ResourceLoader>(&resource_loader);
            service_locator.set<Renderer>(&renderer);
            service_locator.set<ImGuiRenderer>(&imgui_renderer);

            Storytime storytime(
                config,
                &service_locator,
                &event_manager
            );

            on_create(storytime);
            ST_LOG_INFO("Created client");

            //
            // GAME LOOP
            //

            GameLoopStatistics game_loop_stats{};

            auto ms = [](const Duration& duration) -> f64 {
                return Time::as<Nanoseconds>(duration).count() / 1000000.0;
            };

            auto get_duration_ms = [](TimePoint start, TimePoint end) -> f64 {
                return Time::as<Nanoseconds>(end - start).count() / 1000000.0;
            };

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
                f64 last_cycle_duration_ms = Time::as<Nanoseconds>(cycle_start_time - last_cycle_start_time).count() / 1000000.0;

                // If the last cycle lasted too long, assume that we have resumed from a breakpoint
                // and override the duration to the target timestep to avoid big spikes in game systems.
                if (last_cycle_duration_ms > 1000.0) {
                    last_cycle_duration_ms = timestep_ms;
                }

                last_cycle_start_time = cycle_start_time;
                game_clock_lag_ms += last_cycle_duration_ms;

                //
                // PROCESS INPUT
                //

                window.process_events();

                //
                // UPDATE
                //

                i32 update_count = 0;
                f64 update_start_lag_ms = game_clock_lag_ms;
                TimePoint update_start_time = Time::now();

                // Update game clock at fixed timesteps to have game systems update at a
                // predictable rate. Whenever the game clock lags behind the app clock by
                // one-or-more timesteps, tick the game clock forwards until it's caught up.
                while (game_clock_lag_ms >= timestep_ms) {
                    on_update(timestep_sec);
                    game_clock_lag_ms -= timestep_ms;
                    update_count++;
                }

                TimePoint update_end_time = Time::now();
                f64 update_end_lag_ms = game_clock_lag_ms;

                // Process game events between updating and rendering to have any changes in the game state
                // be rendered in the same cycle.
                if (update_count > 0) {
                    event_manager.process_events();
                }

                //
                // RENDER
                //

#ifdef ST_RENDER_IMGUI
                imgui_renderer.begin_frame();
#endif

                TimePoint render_start_time = Time::now();
                renderer.begin_frame();
                on_render();
                renderer.end_frame();
                TimePoint render_end_time = Time::now();
                game_loop_stats.render_duration_ms = ms(render_end_time - render_start_time);

#ifdef ST_RENDER_IMGUI
                TimePoint imgui_render_start_time = Time::now();
                imgui_renderer.render(game_loop_stats);
                on_render_imgui();
                imgui_renderer.end_frame();
                TimePoint imgui_render_end_time = Time::now();
#endif

                window.swap_buffers();
                TimePoint cycle_end_time = Time::now();

                //
                // STATISTICS
                //

                if (update_count > 0) {
                    game_loop_stats.update_timestep_ms = update_start_lag_ms - update_end_lag_ms;
                    game_loop_stats.updates_per_second = update_count / (game_loop_stats.update_timestep_ms / 1000.0);
                    game_loop_stats.update_duration_ms = Time::as<Nanoseconds>(update_end_time - update_start_time).count() / 1000000.0;
                }
                game_loop_stats.render_duration_ms = Time::as<Nanoseconds>(render_end_time - render_start_time).count() / 1000000.0;
                game_loop_stats.frames_per_second = 1.0 / (game_loop_stats.render_duration_ms / 1000.0);
                game_loop_stats.imgui_render_duration_ms = Time::as<Nanoseconds>(imgui_render_end_time - imgui_render_start_time).count() / 1000000.0;
                game_loop_stats.cycle_duration_ms = Time::as<Nanoseconds>(cycle_end_time - cycle_start_time).count() / 1000000.0;
            }

            ST_LOG_INFO("Terminating...");

            on_destroy();
            ST_LOG_INFO("Destroyed client");

            event_manager.unsubscribe_and_clear(event_subscriptions);
            ST_LOG_DEBUG("Unsubscribed event listeners");

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
}
