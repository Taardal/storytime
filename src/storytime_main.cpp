#include "storytime_main.h"
#include "storytime_app.h"

namespace Storytime {
    void run(const Config& config, const std::function<App*(const Config&)>& create_app) {
#ifdef ST_TRACK_MEMORY
        std::atexit(MemoryTracker::terminate);
#endif
        initialize_error_signal_handlers();
        set_log_level(config.log_level);
        App* app = nullptr;
        try {
            app = create_app(config);
            app->run();
        } catch (const Error& e) {
            ST_LOG_CRITICAL("Fatal error");
            e.print_stacktrace();
        } catch (const std::exception& e) {
            ST_LOG_CRITICAL("Fatal error: {}", e.what());
        }
        delete app;
    }
}
