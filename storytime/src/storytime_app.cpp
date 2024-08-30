#include "storytime_app.h"
#include "system/clock.h"
#include "graphics/open_gl.h"

#ifdef ST_DEBUG
#define SHOW_FRAME_STATS
#endif

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
        event_manager.subscribe(EventType::WindowClose, [&](const Event& event) {
            stop();
        });
        service_locator.set<Window>(&window);
        service_locator.set<EventManager>(&event_manager);
        service_locator.set<AudioEngine>(&audio_engine);
        service_locator.set<ResourceLoader>(&resource_loader);
        service_locator.set<Renderer>(&renderer);
        service_locator.set<ImGuiRenderer>(&imgui_renderer);
        service_locator.set<Camera>(&camera);
    }

    const Config& App::get_config() const {
        return config;
    }

    const CommandLineArguments& App::get_args() const {
        return config.command_line_arguments;
    }

    void App::run() {
        running = true;
        on_initialize();
        game_loop();
        on_terminate();
    }

    void App::stop() {
        running = false;
    }

    void App::game_loop() {
        f64 timestep = 0.0;
        f64 last_uptime_sec = 0.0;
        f64 target_frame_sec = 1.0 / config.target_fps;
        f64 target_stats_sec = 1.0;

#ifdef SHOW_FRAME_STATS
        f64 frame_stats_timestep = 0.0;
        u32 ups = 0;
        u32 fps = 0;
#endif

        Clock clock;
        clock.start();

        while (running) {
            window.update();

            f64 uptime_sec = clock.get_time<ms>() / 1000;
            f64 uptime_delta = std::min(uptime_sec - last_uptime_sec, 1.0);
            last_uptime_sec = uptime_sec;

            timestep += uptime_delta;
            if (timestep >= target_frame_sec) {
                on_update(&camera, timestep);
                event_manager.process_event_queue();
                timestep = 0;
                ups++;
            }

            renderer.begin_frame(camera.get_view_projection());
            on_render(&renderer);
            fps++;
            renderer.end_frame();

            imgui_renderer.begin_frame();
            on_imgui_render();
            imgui_renderer.end_frame();

#ifdef SHOW_FRAME_STATS
            frame_stats_timestep += uptime_delta;
            if (frame_stats_timestep >= target_stats_sec) {
                std::stringstream ss;
                ss << "FPS: " << fps << ", UPS: " << ups;
                std::string title = ss.str();
                window.set_title(title.c_str());
                ups = 0;
                fps = 0;
                frame_stats_timestep = 0;
            }
#endif
        }
    }
}
