#include "storytime_main.h"
#include "storytime_engine.h"
#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"
#include "audio/audio_module.h"
#include "resource/resource_module.h"

namespace Storytime {
    void run(const Config& config, const std::function<App*(const Storytime&)>& create_app) {
        App* app = nullptr;
        try {
            SystemModule::initialize(config);
            SystemModule system_module;

            WindowModule::initialize();
            WindowModule window_module(config);

            AudioModule::initialize();
            AudioModule audio_module;

            ResourceModule::initialize();
            ResourceModule resource_module(&system_module, &audio_module);

            GraphicsModule::initialize(config);
            GraphicsModule graphics_module(config, &window_module, &resource_module);

            Engine engine(
                config,
                &window_module,
                &graphics_module
            );

            Storytime storytime(
                config,
                &system_module,
                &window_module,
                &audio_module,
                &resource_module,
                &graphics_module
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

        delete app;

        GraphicsModule::terminate();
        ResourceModule::terminate();
        AudioModule::terminate();
        WindowModule::terminate();
        SystemModule::terminate();
    }
}
