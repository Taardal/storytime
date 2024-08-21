#include "storytime_engine.h"
#include "system/clock.h"
#include "window/window_event.h"

#ifdef ST_DEBUG
    #define SHOW_FRAME_STATS
#endif


namespace Storytime {
    Engine::Engine(const EngineConfig& config) : config(config) {
        config.event_manager->subscribe(EventType::WindowClose, [&](const Event& event) {
            stop();
        });
    }

    void Engine::run(App* app) {
        running = true;

        Clock clock;
        clock.start();

        f64 last_uptime_sec = 0.0;
        f64 target_frame_sec = 1.0 / config.target_fps;
        f64 target_stats_sec = 1.0;

        f64 timestep = 0.0;

#ifdef SHOW_FRAME_STATS
        f64 frame_stats_timestep = 0.0;
        u32 ups = 0;
        u32 fps = 0;
#endif

        while (running) {
            config.window->update();

            f64 uptime_sec = clock.get_time<ms>() / 1000;
            f64 uptime_delta = std::min(uptime_sec - last_uptime_sec, 1.0);
            last_uptime_sec = uptime_sec;

            timestep += uptime_delta;
            if (timestep >= target_frame_sec) {
                app->on_update(config.camera, timestep);
                timestep = 0;
                ups++;
            }

            config.renderer->begin_frame(config.camera->get_view_projection());
            app->on_render(config.renderer);
            fps++;
            config.renderer->end_frame();

            auto [window_width, window_height] = config.window->get_size_in_pixels();

            config.imgui_renderer->begin_frame();
            app->on_imgui_render();
            config.imgui_renderer->end_frame(window_width, window_height);

#ifdef SHOW_FRAME_STATS
            frame_stats_timestep += uptime_delta;
            if (frame_stats_timestep >= target_stats_sec) {
                std::stringstream ss;
                ss << "FPS: " << fps << ", UPS: " << ups;
                std::string title = ss.str();
                config.window->set_title(title.c_str());
                ups = 0;
                fps = 0;
                frame_stats_timestep = 0;
            }
#endif
        }
    }

    void Engine::stop() {
        running = false;
    }
}
