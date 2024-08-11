#include "run.h"
#include "system/error_signal.h"
#include "graphics/GraphicsLog.h"

extern void run_app(const Storytime::NewApplicationConfig&);

void Storytime::run(const NewApplicationConfig& config) {
    initialize_error_signal_handlers();
    set_log_level(config.log_level);
    GraphicsLog::Init({
        .VersionMajor = config.open_gl_version_major,
        .VersionMinor = config.open_gl_version_minor,
        .GlslVersion = config.glsl_version,
    });
    try {
        ST_LOG_I("Running app...");
        run_app(config);
    } catch (const Error& e) {
        ST_LOG_C("Fatal error");
        e.printStacktrace();
    } catch (const std::exception& e) {
        ST_LOG_C("Fatal error: {}", e.what());
    }
}
