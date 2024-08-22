#include "storytime_main.h"
#include "storytime_engine.h"
#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"

void Storytime::run(const Config& config, const std::function<App*(const Storytime&)>& create_app) {
    App* app = nullptr;
    try {
        SystemModule::initialize(config);
        SystemModule system_module;

        WindowModule::initialize();
        WindowModule window_module(config, &system_module);

        GraphicsModule::initialize(config);
        GraphicsModule graphics_module(config, &system_module, &window_module);

        Engine engine(
            config,
            &window_module.window,
            &window_module.event_manager,
            &graphics_module.camera,
            &graphics_module.renderer,
            &graphics_module.imgui_renderer
        );

        Storytime storytime(
            config,
            &system_module.service_locator,
            &window_module.event_manager
        );

        app = create_app(storytime);
        ST_ASSERT(app != nullptr);
        engine.run(app);

    } catch (const Error& e) {
        ST_LOG_CRITICAL("Fatal error");
        e.print_stacktrace();
    } catch (const std::exception& e) {
        ST_LOG_CRITICAL("Fatal error: {}", e.what());
    }

    GraphicsModule::terminate();
    WindowModule::terminate();
    SystemModule::terminate();

    delete app;
}
