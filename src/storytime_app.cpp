#include "storytime_app.h"
#include "system/clock.h"
#include "graphics/open_gl.h"

namespace Storytime {
    App::App(const Config& config)
        : config(config),
          service_locator(),
          file_system(),
          event_manager({
              .queue_count = 1,
          }),
          window({
              .event_manager = &event_manager,
              .title = config.window_title,
              .width = config.window_width,
              .height = config.window_height,
              .maximized = config.window_maximized,
              .resizable = config.window_resizable,
              .context_version_major = config.open_gl_version_major,
              .context_version_minor = config.open_gl_version_minor,
          }),
          audio_engine(),
          resource_loader({
              .file_system = &file_system,
              .audio_engine = &audio_engine
          }),
          open_gl({
              .window = &window,
              .log_level = config.log_level,
              .major_version = config.open_gl_version_major,
              .minor_version = config.open_gl_version_minor,
              .glsl_version = config.glsl_version,
          }),
          renderer(&resource_loader),
          imgui_renderer({
              .window = &window,
              .glsl_version = config.glsl_version,
          }),
          camera()
    {
        service_locator.set<Window>(&window);
        service_locator.set<EventManager>(&event_manager);
        service_locator.set<AudioEngine>(&audio_engine);
        service_locator.set<ResourceLoader>(&resource_loader);
        service_locator.set<Renderer>(&renderer);
        service_locator.set<ImGuiRenderer>(&imgui_renderer);
        service_locator.set<Camera>(&camera);

        event_manager.subscribe(EventType::WindowClose, [&](const Event& event) {
            stop();
        });

        renderer.set_clear_color({0.1f, 0.1f, 0.1f});

        camera.set_projection({
            .left = 0,
            .right = static_cast<float>(config.window_width),
            .top = 0,
            .bottom = static_cast<float>(config.window_height),
        });
    }

    const Config& App::get_config() const {
        return config;
    }

    const CommandLineArguments& App::get_args() const {
        return config.command_line_arguments;
    }

    void App::run() {
        running = true;
        ST_EXECUTE_THROW(on_initialize());
        ST_EXECUTE_THROW(game_loop());
        ST_EXECUTE_THROW(on_terminate());
    }

    void App::stop() {
        running = false;
    }

    void App::game_loop() {
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
            on_imgui_render();
            imgui_renderer.end_frame();
#endif

            frame_statistics_timestep += timestep;
            if (frame_statistics_timestep >= 1.0) {
                frame_statistics_timestep = 0;
                on_statistics(statistics);
                statistics.ups = 0;
                statistics.fps = 0;
            }
        }
    }
}
