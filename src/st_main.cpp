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
#include "system/frame_info.h"
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
                    auto& window_resize_event = static_cast<const WindowResizeEvent&>(event);
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

            GameLoopStatistics game_loop_stats{};


            // Use fixed timestep to have game systems always update at a predictable rate
            constexpr f64 timestep_sec = 1.0 / 60.0;
            constexpr f64 timestep_ms = timestep_sec * 1000.0;

            Clock clock;
            clock.start();

            Time last_cycle_start_time = clock.now();
            f64 game_clock_lag_ms = 0.0; // How far the game clock is behind the app clock

            running = true;
            ST_LOG_INFO("Running...");

            while (running) {
                Time cycle_start_time = clock.now();

                // If the last cycle lasted too long, assume that we have resumed from a breakpoint
                // and force it to the target rate for this frame to avoid big spikes in game systems.
                f64 last_cycle_duration_ms = clock.duration<ns>(cycle_start_time - last_cycle_start_time).count() / 1000000.0;
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
                Time update_start_time = clock.now();

                // Update game clock at fixed timesteps to have game systems always update at a predictable rate
                while (game_clock_lag_ms >= timestep_ms) {
                    on_update(timestep_sec);
                    game_clock_lag_ms -= timestep_ms;
                    update_count++;
                }

                f64 update_end_lag_ms = game_clock_lag_ms;
                Time update_end_time = clock.now();

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

                Time render_start_time = clock.now();
                renderer.begin_frame();
                on_render();
                renderer.end_frame();
                Time render_end_time = clock.now();

#ifdef ST_RENDER_IMGUI
                Time imgui_render_start_time = clock.now();
                imgui_renderer.render(game_loop_stats);
                on_render_imgui();
                imgui_renderer.end_frame();
                Time imgui_render_end_time = clock.now();
#endif

                window.next_frame();
                Time cycle_end_time = clock.now();

                //
                // STATISTICS
                //

                static constexpr f64 low_res_smoothing_factor = 0.1;
                static constexpr f64 high_res_smoothing_factor = 0.01;

                f64 cycle_duration_ms = clock.duration<ns>(cycle_end_time - cycle_start_time).count() / 1000000.0;
                game_loop_stats.cycle_duration_ms = smooth_average(cycle_duration_ms, game_loop_stats.cycle_duration_ms, high_res_smoothing_factor);

                if (update_count > 0) {
                    f64 update_duration_ms = clock.duration<ns>(update_end_time - update_start_time).count() / 1000000.0;
                    game_loop_stats.update_duration_ms = smooth_average(update_duration_ms, game_loop_stats.update_duration_ms, high_res_smoothing_factor);

                    f64 update_timestep_ms = update_start_lag_ms - update_end_lag_ms;
                    game_loop_stats.update_timestep_ms = smooth_average(update_timestep_ms, game_loop_stats.update_timestep_ms, low_res_smoothing_factor);

                    f64 updates_per_second = update_count / (update_timestep_ms / 1000.0);
                    game_loop_stats.updates_per_second = smooth_average(updates_per_second, game_loop_stats.updates_per_second, low_res_smoothing_factor);
                }

                f64 render_duration_ms = clock.duration<ns>(render_end_time - render_start_time).count() / 1000000.0;
                game_loop_stats.render_duration_ms = smooth_average(render_duration_ms, game_loop_stats.render_duration_ms, high_res_smoothing_factor);

                f64 frames_per_second = 1.0 / (render_duration_ms / 1000.0);
                game_loop_stats.frames_per_second = smooth_average(frames_per_second, game_loop_stats.frames_per_second, low_res_smoothing_factor);

                f64 imgui_render_duration_ms = clock.duration<ns>(imgui_render_end_time - imgui_render_start_time).count() / 1000000.0;
                game_loop_stats.imgui_render_duration_ms = smooth_average(imgui_render_duration_ms, game_loop_stats.imgui_render_duration_ms, high_res_smoothing_factor);
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
