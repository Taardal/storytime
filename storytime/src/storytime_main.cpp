#include "storytime_main.h"
#include "system/error_signal.h"
#include "graphics/GraphicsLog.h"

extern Storytime::StorytimeApp* create_app(const Storytime::AppConfig&);

void Storytime::run(int argc, char *argv[], const AppConfig& config) {
    initialize_error_signal_handlers();
    set_log_level(config.log_level);
    GraphicsLog::Init({
        .VersionMajor = config.open_gl_version_major,
        .VersionMinor = config.open_gl_version_minor,
        .GlslVersion = config.glsl_version,
    });
    StorytimeApp* app = nullptr;
    try {
        ST_LOG_INFO("Initializing app...");
        app = create_app(config);
    } catch (const Error& e) {
        ST_LOG_CRITICAL("Initialization error");
        e.print_stacktrace();
    } catch (const std::exception& e) {
        ST_LOG_CRITICAL("Initialization error: {}", e.what());
    }
    if (app != nullptr) {
        try {
            ST_LOG_INFO("Running app...");
            app->run();
        } catch (const Error& e) {
            ST_LOG_CRITICAL("Runtime error");
            e.print_stacktrace();
        } catch (const std::exception& e) {
            ST_LOG_CRITICAL("Runtime error: {}", e.what());
        }
    }
    delete app;
}
