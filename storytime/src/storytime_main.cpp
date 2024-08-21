#include "storytime_main.h"
#include "storytime_engine.h"
#include "system/memory.h"
#include "window/user_input.h"
#include "graphics/open_gl.h"

#include "system/service_locator.h"

void Storytime::run(const Config& config, const std::function<App*(const Storytime&)>& create_app) {
#ifdef ST_TRACK_MEMORY
    std::atexit(MemoryTracker::terminate);
#endif
    initialize_error_signal_handlers();
    set_log_level(config.log_level);

    App* app = nullptr;
    try {
        ST_LOG_INFO("Initializing app...");

        // System
        ServiceLocator service_locator;
        FileSystem file_system;
        ResourceLoader resource_loader(&file_system);

        // Window
        EventManager event_manager;
        Window window({
            .event_manager = &event_manager,
            .title = config.window_title,
            .width = config.window_width,
            .height = config.window_height,
            .maximized = config.window_maximized,
            .resizable = config.window_resizable,
            .context_version_major = config.open_gl_version_major,
            .context_version_minor = config.open_gl_version_minor,
        });

        //Graphics
        OpenGL open_gl({
            .major_version = config.open_gl_version_major,
            .minor_version = config.open_gl_version_minor,
            .glsl_version = config.glsl_version,
        });
        Renderer renderer(&resource_loader);
        ImGuiRenderer imgui_renderer({
            .window = &window,
            .glsl_version = config.glsl_version,
        });
        Camera camera;

        Engine engine({
            .target_fps = config.target_fps,
            .service_locator = &service_locator,
            .window = &window,
            .event_manager = &event_manager,
            .renderer = &renderer,
            .imgui_renderer = &imgui_renderer,
            .camera = &camera,
        });

        service_locator.set<ResourceLoader>(&resource_loader);
        service_locator.set<EventManager>(&event_manager);
        service_locator.set<Window>(&window);
        service_locator.set<Renderer>(&renderer);
        service_locator.set<ImGuiRenderer>(&imgui_renderer);
        service_locator.set<Camera>(&camera);

        Storytime storytime(config, &service_locator, &event_manager);
        app = create_app(storytime);

        engine.run(app);

    } catch (const Error& e) {
        ST_LOG_CRITICAL("Fatal error");
        e.print_stacktrace();
    } catch (const std::exception& e) {
        ST_LOG_CRITICAL("Fatal error: {}", e.what());
    }

    delete app;
}
