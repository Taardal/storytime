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
            ServiceLocator service_locator;

            EventManager event_manager({
                .queue_count = 1,
            });
            service_locator.set<EventManager>(&event_manager);

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
            service_locator.set<Window>(&window);

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
                event_manager.subscribe(WindowResizeEvent::type, [&renderer](const Event& event) {
                    auto& window_resize_event = (WindowResizeEvent&) event;
                    renderer.set_viewport({
                        .width = window_resize_event.width,
                        .height = window_resize_event.height,
                    });
                })
            );
            service_locator.set<Renderer>(&renderer);

#ifdef ST_IMGUI_ENABLED
            ImGuiRenderer imgui_renderer({
                .glsl_version = config.glsl_version,
                .window = &window,
                .event_manager = &event_manager,
            });
            service_locator.set<ImGuiRenderer>(&imgui_renderer);

            // When rendering ImGui, we need to render the game to a specific ImGui window,
            // instead of the application window. We accomplish this by first rendering the
            // game to this framebuffer, and then render that framebuffer to the ImGui window.
            Framebuffer imgui_framebuffer({
                .width = (u32) window_size_px.width,
                .height = (u32) window_size_px.height,
            });
            event_subscriptions.push_back(
                event_manager.subscribe(WindowResizeEvent::type, [&imgui_framebuffer](const Event& event) {
                    auto& window_resize_event = (WindowResizeEvent&) event;
                    imgui_framebuffer.resize(window_resize_event.width, window_resize_event.height);
                })
            );
            event_subscriptions.push_back(
                event_manager.subscribe(ImGuiWindowResizeEvent::type, [&imgui_framebuffer](const Event& event) {
                    auto& imgui_window_resize_event = (ImGuiWindowResizeEvent&) event;
                    if (imgui_window_resize_event.window_id == ImGuiRenderer::game_window_name) {
                        imgui_framebuffer.resize(imgui_window_resize_event.width, imgui_window_resize_event.height);
                    }
                })
            );
#endif

            Storytime storytime(
                &const_cast<Config&>(config),
                &service_locator,
                &event_manager
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

            GameLoopStatistics game_loop_stats{};

            running = true;
            ST_LOG_INFO("Running...");

            while (running) {
                TimePoint cycle_start_time = Time::now();
                f64 last_cycle_duration_ms = Time::as<Microseconds>(cycle_start_time - last_cycle_start_time).count() / 1000.0;

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

                TimePoint window_event_start_time = Time::now();
                window.process_events();
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

                // Process game events between updating and rendering to have any changes in the game state
                // be rendered in the same cycle.
                TimePoint game_event_start_time = Time::zero();
                TimePoint game_event_end_time = Time::zero();
                if (update_count > 0) {
                    game_event_start_time = Time::now();
                    event_manager.process_events();
                    game_event_end_time = Time::now();
                }

                //
                // RENDER
                //

#ifdef ST_IMGUI_ENABLED
                imgui_framebuffer.bind();
#endif

                TimePoint render_start_time = Time::now();
                renderer.begin_frame();
                on_render();
                renderer.end_frame();
                TimePoint render_end_time = Time::now();

#ifdef ST_IMGUI_ENABLED
                imgui_framebuffer.unbind();

                TimePoint imgui_render_start_time = Time::now();
                imgui_renderer.begin_frame();
                imgui_renderer.render(imgui_framebuffer, game_loop_stats);
                on_render_imgui();
                imgui_renderer.end_frame();
                TimePoint imgui_render_end_time = Time::now();
#endif

                TimePoint swap_buffers_start_time = Time::now();
                window.swap_buffers();
                TimePoint swap_buffers_end_time = Time::now();

                TimePoint cycle_end_time = Time::now();

                //
                // STATISTICS
                //

                if (update_count > 0) {
                    game_loop_stats.update_timestep_ms = update_start_lag_ms - update_end_lag_ms;
                    game_loop_stats.updates_per_second = update_count / (game_loop_stats.update_timestep_ms / 1000.0);
                    game_loop_stats.update_duration_ms = Time::as<Microseconds>(update_end_time - update_start_time).count() / 1000.0;
                }
                game_loop_stats.render_duration_ms = Time::as<Microseconds>(render_end_time - render_start_time).count() / 1000.0;
                game_loop_stats.frames_per_second = 1.0 / (game_loop_stats.render_duration_ms / 1000.0);
#ifdef ST_IMGUI_ENABLED
                game_loop_stats.imgui_render_duration_ms = Time::as<Microseconds>(imgui_render_end_time - imgui_render_start_time).count() / 1000.0;
#endif
                game_loop_stats.window_events_duration_ms = Time::as<Microseconds>(window_event_end_time - window_event_start_time).count() / 1000.0;
                game_loop_stats.game_events_duration_ms = Time::as<Microseconds>(game_event_end_time - game_event_start_time).count() / 1000.0;
                game_loop_stats.swap_buffers_duration_ms = Time::as<Microseconds>(swap_buffers_end_time - swap_buffers_start_time).count() / 1000.0;
                game_loop_stats.cycle_duration_ms = Time::as<Microseconds>(cycle_end_time - cycle_start_time).count() / 1000.0;
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
