#include "st_main.h"

namespace Storytime {
    void main(const Config& config, const std::function<void(Storytime& storytime)>& run_app) {
        initialize_error_signal_handlers();
        initialize_log(config.log_level);

#ifdef ST_TRACK_MEMORY
        initialize_memory_tracking();
        std::atexit(terminate_memory_tracking);
#endif

        Engine engine(config);
        Storytime storytime(config, engine);

        run_app(storytime);
    }
}
