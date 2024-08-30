#include "storytime_main.h"
#include "storytime_engine.h"
#include "system/system_module.h"
#include "window/window_module.h"
#include "graphics/graphics_module.h"
#include "audio/audio_module.h"
#include "resource/resource_module.h"

#include "system/memory_allocator.h"

void Storytime::run(const Config& config, const std::function<App*(const Storytime&)>& create_app) {
    SystemModule* system_module = nullptr;
    WindowModule* window_module = nullptr;

    App* app = nullptr;
    try {
        SystemModule::initialize(config);
        // SystemModule system_module;
        // system_module = new(allocator.allocate(sizeof(SystemModule))) SystemModule();
        // system_module = ST_NEW_MODULE(SystemModule);
        system_module = ST_NEW SystemModule();

        WindowModule::initialize();
        // WindowModule window_module(config, system_module);
        window_module = ST_NEW WindowModule(config, system_module);

        // AudioModule audio_module(system_module);
        //
        // ResourceModule resource_module(system_module, &audio_module);
        //
        // GraphicsModule::initialize(config);
        // GraphicsModule graphics_module(config, system_module, &window_module, &resource_module);
        //
        // Engine engine(
        //     config,
        //     window_module.get<Window>(),
        //     window_module.get<EventManager>(),
        //     graphics_module.get<Camera>(),
        //     graphics_module.get<Renderer>(),
        //     graphics_module.get<ImGuiRenderer>()
        // );
        //
        // Storytime storytime(
        //     config,
        //     system_module->get<ServiceLocator>(),
        //     window_module.get<EventManager>()
        // );
        //
        // app = create_app(storytime);
        // ST_ASSERT(app != nullptr);
        // engine.run(app);

    } catch (const Error& e) {
        ST_LOG_CRITICAL("Fatal error");
        e.print_stacktrace();
    } catch (const std::exception& e) {
        ST_LOG_CRITICAL("Fatal error: {}", e.what());
    }

    delete app;

    delete window_module;
    WindowModule::terminate();

    // ST_DELETE_MODULE(system_module, SystemModule);
    delete system_module;
}
