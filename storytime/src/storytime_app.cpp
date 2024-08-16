#include "storytime_app.h"
#include "system/clock.h"
#include "window/window_event.h"

namespace Storytime {
    StorytimeApp::StorytimeApp(const AppConfig& config)
        : config(config),
          system_module(),
          window_module({
              .system_module = &system_module,
              .window_config = {
                  .title = config.window_title,
                  .width = config.window_width,
                  .height = config.window_height,
                  .maximized = config.window_maximized,
                  .resizable = config.window_resizable,
                  .context_version_major = config.open_gl_version_major,
                  .context_version_minor = config.open_gl_version_minor,
              },
          }),
          graphics_module({
              .system_module = &system_module,
              .window_module = &window_module,
              .open_gl_config = {
                  .major_version = config.open_gl_version_major,
                  .minor_version = config.open_gl_version_minor,
                  .glsl_version = config.glsl_version,
              },
          })
    {
        window_module.event_manager.subscribe(EventType::WindowClose, [&](const Event& event) {
            stop();
        });
    }

    void StorytimeApp::run() {
        running = true;

        Clock clock;
        clock.start();

        f64 last_uptime_sec = 0.0;
        f64 target_frame_time = 1.0 / config.target_fps;
        f64 timestep = 0.0;

        while (running) {
            window_module.window.update();

            f64 uptime_sec = clock.get_time<ms>() / 1000;
            f64 uptime_delta = std::min(uptime_sec - last_uptime_sec, 1.0);
            last_uptime_sec = uptime_sec;
            timestep += uptime_delta;

            if (timestep >= target_frame_time) {
                on_update(timestep);
                timestep = 0;
            }

            graphics_module.renderer.begin_frame(graphics_module.camera.GetViewProjection());
            on_render();
            graphics_module.renderer.end_frame();

            auto [window_width, window_height] = window_module.window.get_size_in_pixels();

            graphics_module.imgui_renderer.begin_frame();
            on_imgui_render();
            graphics_module.imgui_renderer.end_frame(window_width, window_height);
        }
    }

    void StorytimeApp::stop() {
        running = false;
    }
}
