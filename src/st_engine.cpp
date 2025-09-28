#include "st_engine.h"

#include "st_app.h"
#include "event/st_window_closed_event.h"
#include "system/st_clock.h"

namespace Storytime {
    Engine::Engine(const Config& config)
        : service_locator(),
          dispatcher(),
          file_reader(),
          audio_engine(),
          resource_loader({
              .file_reader = &file_reader,
              .audio_engine = &audio_engine
          }),
          window({
              .dispatcher = &dispatcher,
              .title = config.app_name,
              .width = config.window_width,
              .height = config.window_height,
              .aspect_ratio = config.window_aspect_ratio,
              .fullscreen = config.window_fullscreen,
              .maximized = config.window_maximized,
              .resizable = config.window_resizable,
              .vsync = config.window_vsync,
              .context_version_major = config.open_gl_version_major,
              .context_version_minor = config.open_gl_version_minor,
          }),
#ifndef ST_USE_VULKAN
          open_gl({
            .window = &window,
            .log_level = config.log_level,
            .major_version = config.open_gl_version_major,
            .minor_version = config.open_gl_version_minor,
            .glsl_version = config.glsl_version,
          }),
          renderer({
              .dispatcher = &dispatcher,
              .resource_loader = &resource_loader,
              .window = &window,
          }),
          imgui_renderer({
            .window = &window,
            .keyboard = &keyboard,
            .mouse = &mouse,
            .settings_file_path = config.imgui_settings_file_path,
            .glsl_version = config.glsl_version,
          }),
#endif
          keyboard({
              .window = &window,
              .dispatcher = &dispatcher,
          }),
          mouse({
              .window = &window,
          }),
          process_manager(),
          game_loop_metrics(),
          vulkan_renderer({
              .dispatcher = &dispatcher,
              .window = &window,
              .name = config.app_name,
              .max_frames_in_flight = config.rendering_buffer_count,
              .validation_layers_enabled = config.vulkan_validation_layers_enabled,
          })
    {
        service_locator.set<Dispatcher>(&dispatcher);
        service_locator.set<Window>(&window);
        service_locator.set<Keyboard>(&keyboard);
        service_locator.set<Mouse>(&mouse);
        service_locator.set<FileReader>(&file_reader);
        service_locator.set<ResourceLoader>(&resource_loader);
#ifndef ST_USE_VULKAN
        service_locator.set<Renderer>(&renderer);
#endif
        service_locator.set<ProcessManager>(&process_manager);
        service_locator.set<GameLoopMetrics>(&game_loop_metrics);

        dispatcher.subscribe<WindowClosedEvent>([&](const WindowClosedEvent&) {
            stop();
        });
    }

    void Engine::run(App& app) {
        running = true;
        run_game_loop(app);
    }

    void Engine::stop() {
        running = false;
    }

    void Engine::run_game_loop(App& app) {
        // Update game at fixed timesteps to have game systems update at a predictable rate
        constexpr f64 timestep_sec = 1.0 / 60.0;
        constexpr f64 timestep_ms = timestep_sec * 1000.0;

        // How far the game clock is behind the app clock
        f64 game_clock_lag_ms = 0.0;

        // Use the duration of the last game loop cycle to increment game clock lag
        TimePoint last_cycle_start_time = Time::now();

        while (running) {

            //
            // BEGIN FRAME
            //

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
            // PROCESS EVENTS
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
                // app.update(timestep_sec);
                game_clock_lag_ms -= timestep_ms;
                update_count++;
            }

            TimePoint update_end_time = Time::now();
            f64 update_end_lag_ms = game_clock_lag_ms;

            //
            // RENDER
            //

            TimePoint render_start_time = Time::now();
#ifndef ST_USE_VULKAN
            renderer.begin_frame();
            app.render();
            renderer.end_frame();
#else
            vulkan_renderer.begin_frame();
            vulkan_renderer.render();
            vulkan_renderer.end_frame();
#endif
            TimePoint render_end_time = Time::now();

#ifndef ST_USE_VULKAN
            TimePoint imgui_render_start_time = Time::now();
            imgui_renderer.begin_frame();
            app.render_imgui();
            imgui_renderer.end_frame();
            TimePoint imgui_render_end_time = Time::now();

            //
            // END FRAME
            //

            TimePoint swap_buffers_start_time = Time::now();
            window.swap_buffers();
            TimePoint swap_buffers_end_time = Time::now();
#endif
            TimePoint cycle_end_time = Time::now();

            //
            // METRICS
            //

            if (update_count > 0) {
                game_loop_metrics.update_timestep_ms = update_start_lag_ms - update_end_lag_ms;
                game_loop_metrics.updates_per_second = update_count / (game_loop_metrics.update_timestep_ms / 1000.0);
                game_loop_metrics.update_duration_ms = Time::as<Microseconds>(update_end_time - update_start_time).count() / 1000.0;
            }
            game_loop_metrics.render_duration_ms = Time::as<Microseconds>(render_end_time - render_start_time).count() / 1000.0;
            game_loop_metrics.frames_per_second = 1.0 / (game_loop_metrics.render_duration_ms / 1000.0);
#ifndef ST_USE_VULKAN
            game_loop_metrics.imgui_render_duration_ms = Time::as<Microseconds>(imgui_render_end_time - imgui_render_start_time).count() / 1000.0;
#endif
            game_loop_metrics.window_events_duration_ms = Time::as<Microseconds>(window_event_end_time - window_event_start_time).count() / 1000.0;
#ifndef ST_USE_VULKAN
            game_loop_metrics.swap_buffers_duration_ms = Time::as<Microseconds>(swap_buffers_end_time - swap_buffers_start_time).count() / 1000.0;
#endif
            game_loop_metrics.cycle_duration_ms = Time::as<Microseconds>(cycle_end_time - cycle_start_time).count() / 1000.0;
        }
    }
}
