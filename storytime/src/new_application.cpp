#include "new_application.h"
#include "system/clock.h"

namespace Storytime {
    NewApplication::NewApplication(const NewApplicationConfig& config)
        : system_module(),
          window_module({
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
              .context_config = {
                  .VersionMajor = config.open_gl_version_major,
                  .VersionMinor = config.open_gl_version_minor,
                  .GlslVersion = config.glsl_version,
              },
          }),
          window(&window_module.window),
          renderer(&graphics_module.renderer)
    {
        ST_LOG_TRACE(ST_TAG, "Created app");
    }

    void NewApplication::run() {
        ST_LOG_TRACE(ST_TAG, "Running...");
        running = true;

        Clock clock;
        clock.start();

        f64 last_uptime_sec = 0.0;
        f64 target_frame_time = 1.0 / 60.0;
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

            graphics_module.renderer.BeginScene(graphics_module.camera.GetViewProjection());
            on_render();
            graphics_module.renderer.EndScene();
        }
    }

    void NewApplication::stop() {
        running = false;
    }
}
