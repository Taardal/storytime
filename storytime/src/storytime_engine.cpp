#include "storytime_engine.h"
#include "system/clock.h"

#ifdef ST_DEBUG
#define SHOW_FRAME_STATS
#endif

namespace Storytime {
    Engine::Engine(
        const Config& config,
        WindowModule* window_module,
        GraphicsModule* graphics_module
    ) : config(config),
        window(window_module->get<Window>()),
        camera(graphics_module->get<Camera>()),
        renderer(graphics_module->get<Renderer>()),
        imgui_renderer(graphics_module->get<ImGuiRenderer>())
    {
        window_module->get<EventManager>()->subscribe(EventType::WindowClose, [&](const Event& event) {
            stop();
        });
    }

    void Engine::run(App* app) {
        app->on_initialize();

        f64 timestep = 0.0;
        f64 last_uptime_sec = 0.0;
        f64 target_frame_sec = 1.0 / config.target_fps;
        f64 target_stats_sec = 1.0;

#ifdef SHOW_FRAME_STATS
        f64 frame_stats_timestep = 0.0;
        u32 ups = 0;
        u32 fps = 0;
#endif

        app->on_initialize();

        Clock clock;
        clock.start();

        running = true;

        while (running) {
            window->update();

            f64 uptime_sec = clock.get_time<ms>() / 1000;
            f64 uptime_delta = std::min(uptime_sec - last_uptime_sec, 1.0);
            last_uptime_sec = uptime_sec;

            timestep += uptime_delta;
            if (timestep >= target_frame_sec) {
                app->on_update(camera, timestep);
                timestep = 0;
                ups++;
            }

            renderer->begin_frame(camera->get_view_projection());
            app->on_render(renderer);
            fps++;
            renderer->end_frame();

            imgui_renderer->begin_frame();
            app->on_imgui_render();
            imgui_renderer->end_frame();

#ifdef SHOW_FRAME_STATS
            frame_stats_timestep += uptime_delta;
            if (frame_stats_timestep >= target_stats_sec) {
                std::stringstream ss;
                ss << "FPS: " << fps << ", UPS: " << ups;
                std::string title = ss.str();
                window->set_title(title.c_str());
                ups = 0;
                fps = 0;
                frame_stats_timestep = 0;
            }
#endif
        }
        app->on_terminate();
    }

    void Engine::stop() {
        running = false;
    }
}
