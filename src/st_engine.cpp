#include "st_engine.h"

#include "graphics/st_vulkan_swapchain.h"
#include "st_app.h"
#include "event/st_window_closed_event.h"
#include "system/st_clock.h"

namespace Storytime {
    typedef Renderer Renderer;

    Engine::Engine(const Config& config)
        : service_locator(),
          dispatcher(),
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
          }),
          keyboard({
              .window = &window,
              .dispatcher = &dispatcher,
          }),
          mouse({
              .window = &window,
          }),
          file_reader(),
          metrics(),
          vulkan_context({
              .window = &window,
              .api_version = config.vulkan_api_version,
              .app_name = config.vulkan_app_name.size() > 0 ? config.vulkan_app_name : config.app_name,
              .app_version = config.vulkan_app_version,
              .engine_name = config.vulkan_engine_name.size() > 0 ? config.vulkan_engine_name : std::format("{} Engine", config.app_name),
              .engine_version = config.vulkan_engine_version,
              .validation_layers_enabled = config.vulkan_validation_layers_enabled,
          }),
          vulkan_physical_device({
              .context = &vulkan_context,
          }),
          vulkan_device({
              .name = std::format("{} device", config.app_name),
              .physical_device = &vulkan_physical_device,
          }),
          vulkan_swapchain({
              .name = std::format("{} swapchain", config.app_name),
              .dispatcher = &dispatcher,
              .window = &window,
              .device = &vulkan_device,
              .surface = vulkan_context.get_surface(),
          }),
          renderer({
              .name = std::format("{} renderer", config.app_name),
              .dispatcher = &dispatcher,
              .window = &window,
              .file_reader = &file_reader,
              .metrics = &metrics,
              .context = &vulkan_context,
              .device = &vulkan_device,
              .swapchain = &vulkan_swapchain,
              .frame_count = config.rendering_buffer_count,
          }),
          imgui_renderer({
              .name = std::format("{} imgui renderer", config.app_name),
              .window = &window,
              .keyboard = &keyboard,
              .mouse = &mouse,
              .context = &vulkan_context,
              .physical_device = &vulkan_physical_device,
              .device = &vulkan_device,
              .swapchain = &vulkan_swapchain,
              .api_version = config.vulkan_api_version,
              .frame_count = config.rendering_buffer_count,
              .settings_file_path = config.imgui_settings_file_path,
          }),
          audio_engine(),
          resource_loader({
              .file_reader = &file_reader,
              .audio_engine = &audio_engine,
              .vulkan_device = &vulkan_device,
          }),
          process_manager()
    {
        service_locator.set<Dispatcher>(&dispatcher);
        service_locator.set<Window>(&window);
        service_locator.set<Keyboard>(&keyboard);
        service_locator.set<Mouse>(&mouse);
        service_locator.set<FileReader>(&file_reader);
        service_locator.set<ResourceLoader>(&resource_loader);
        service_locator.set<Renderer>(&renderer);
        service_locator.set<ProcessManager>(&process_manager);
        service_locator.set<Metrics>(&metrics);

        dispatcher.subscribe<WindowClosedEvent>([&](const WindowClosedEvent&) {
            stop();
        });
    }

    void Engine::run(App& app) {
        running = true;
        run_game_loop(app);
        renderer.wait_until_idle();
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

            u32 update_count = 0;
            f64 update_start_lag_ms = game_clock_lag_ms;
            TimePoint update_start_time = Time::now();

            // Update game clock at fixed timesteps to have game systems update at a predictable rate.
            // Whenever the game clock lags behind the app clock by one-or-more timesteps, tick the game
            // clock forwards until it's caught up.
            while (game_clock_lag_ms >= timestep_ms) {
                app.update(timestep_sec);
                game_clock_lag_ms -= timestep_ms;
                update_count++;
            }

            TimePoint update_end_time = Time::now();
            f64 update_end_lag_ms = game_clock_lag_ms;

            //
            // RENDER
            //

            TimePoint render_start_time;
            TimePoint render_end_time;
            TimePoint imgui_render_start_time;
            TimePoint imgui_render_end_time;

            const Frame* frame = renderer.begin_frame();
            if (frame) {
                render_start_time = Time::now();

                renderer.begin_render();
                app.render();
                renderer.end_render();

                imgui_render_start_time = Time::now();
                imgui_renderer.begin_render();
                app.render_imgui();
                imgui_renderer.end_render(frame->command_buffer);
                imgui_render_end_time = Time::now();

                renderer.end_frame();
                render_end_time = Time::now();
            }

            //
            // END FRAME
            //

            TimePoint cycle_end_time = Time::now();

            //
            // METRICS
            //

            if (update_count > 0) {
                metrics.update_timestep_ms = update_start_lag_ms - update_end_lag_ms;
                metrics.updates_per_second = update_count / (metrics.update_timestep_ms / 1000.0);
                metrics.update_duration_ms = Time::as<Microseconds>(update_end_time - update_start_time).count() / 1000.0;
            }
            if (frame) {
                metrics.render_duration_ms = Time::as<Microseconds>(render_end_time - render_start_time).count() / 1000.0;
                metrics.imgui_render_duration_ms = Time::as<Microseconds>(imgui_render_end_time - imgui_render_start_time).count() / 1000.0;
                metrics.scene_render_duration_ms = metrics.render_duration_ms - metrics.imgui_render_duration_ms;
                metrics.frames_per_second = 1.0 / (metrics.render_duration_ms / 1000.0);
            }
            metrics.window_events_duration_ms = Time::as<Microseconds>(window_event_end_time - window_event_start_time).count() / 1000.0;
            metrics.cycle_duration_ms = Time::as<Microseconds>(cycle_end_time - cycle_start_time).count() / 1000.0;

#ifdef ST_DEBUG
            static double window_title_update_lag_sec = 0.0;
            window_title_update_lag_sec += last_cycle_duration_ms / 1000.0;
            if (window_title_update_lag_sec >= 1.0) {
                window_title_update_lag_sec = 0;
                std::string title = std::format("FPS: {}, UPS: {}", (u32) metrics.frames_per_second, (u32) metrics.updates_per_second);
                window.set_title(title.c_str());
            }
#endif
        }
    }
}
