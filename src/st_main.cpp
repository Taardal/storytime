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

            GameLoopStats game_loop_stats{};

            constexpr f64 default_smoothing_factor = 0.1; // Adjust for stability (0 < smoothing_factor ≤ 1)

            auto smooth = [&](f64 time, f64 previous_time, f64 smoothing_factor = default_smoothing_factor) -> f64 {
                return smoothing_factor * time + (1.0 - smoothing_factor) * previous_time; // Exponential Moving Average
            };

            static f64 SEC_PER_UPDATE = 1.0 / 60.0;
            static f64 MS_PER_UPDATE = SEC_PER_UPDATE * 1000.0;

            f64 last_cycle_start_ms = 0.0;
            f64 update_lag_ms = 0.0; // How far the game clock is behind the app clock

            Clock app_clock;
            app_clock.start();

            running = true;
            ST_LOG_INFO("Running...");

            while (running) {

                // Calculate time since last cycle
                f64 cycle_start_ms = app_clock.elapsed<ms>().count();
                f64 ms_since_last_cycle = cycle_start_ms - last_cycle_start_ms;

                // If the time since last cycle is too large, assume that we have resumed from a breakpoint
                // and force it to the target rate this frame to avoid "extreme spikes" in game systems.
                if (ms_since_last_cycle > 1000.0) {
                    ms_since_last_cycle = MS_PER_UPDATE;
                }

                // Update last cycle time for next frame
                last_cycle_start_ms = cycle_start_ms;

                // Update how far the game clock is behind the app clock
                update_lag_ms += ms_since_last_cycle;

                // Process platform and/or user input events
                window.process_events();

                // Update game clock at a fixed timestep to have game systems like physics and AI
                // always update at a predictable rate.
                i32 update_count = 0;
                f64 update_start_lag_ms = update_lag_ms;
                f64 update_start_ms = app_clock.elapsed<ms>().count();
                while (update_lag_ms >= MS_PER_UPDATE) {
                    on_update(SEC_PER_UPDATE);
                    update_lag_ms -= MS_PER_UPDATE;
                    update_count++;
                }
                f64 update_end_ms = app_clock.elapsed<ms>().count();

                // Process game events between updating and rendering to have the game state rendered
                // as accurately as possible. We want to process changes in the game world as soon as
                // possible so they can be rendered in the same cycle.
                if (update_count > 0) {
                    event_manager.process_events();
                }

#ifdef ST_RENDER_IMGUI
                imgui_renderer.begin_frame();
#endif

                f64 render_start_ms = app_clock.elapsed<ms>().count();
                renderer.begin_frame();
                on_render();
                renderer.end_frame();
                f64 render_end_ms = app_clock.elapsed<ms>().count();

#ifdef ST_RENDER_IMGUI
                f64 imgui_render_start_ms = app_clock.elapsed<ms>().count();
                imgui_renderer.render(game_loop_stats);
                on_render_imgui();
                imgui_renderer.end_frame();
                f64 imgui_render_end_ms = app_clock.elapsed<ms>().count();
#endif

                window.next_frame();
                f64 cycle_end_ms = app_clock.elapsed<ms>().count();

                // Update game loop statistics after cycle is complete for use next frame
                if (update_count > 0) {
                    f64 update_duration_ms = update_end_ms - update_start_ms;
                    game_loop_stats.update_duration_ms = smooth(update_duration_ms, game_loop_stats.update_duration_ms);

                    f64 update_timestep_ms = update_start_lag_ms - update_lag_ms;
                    game_loop_stats.update_timestep_ms = smooth(update_timestep_ms, game_loop_stats.update_timestep_ms);

                    f64 updates_per_second = update_count / (update_timestep_ms / 1000.0);
                    game_loop_stats.updates_per_second = smooth(updates_per_second, game_loop_stats.updates_per_second);
                }

                f64 render_duration_ms = render_end_ms - render_start_ms;
                game_loop_stats.render_duration_ms = smooth(render_duration_ms, game_loop_stats.render_duration_ms);

                f64 frames_per_second = 1.0 / (render_duration_ms / 1000.0);
                game_loop_stats.frames_per_second = smooth(frames_per_second, game_loop_stats.frames_per_second);

                f64 imgui_render_time_ms = imgui_render_end_ms - imgui_render_start_ms;
                game_loop_stats.imgui_render_duration_ms = smooth(imgui_render_time_ms, game_loop_stats.imgui_render_duration_ms);

                f64 cycle_duration_ms = cycle_end_ms - cycle_start_ms;
                game_loop_stats.cycle_duration_ms = smooth(cycle_duration_ms, game_loop_stats.cycle_duration_ms);
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
