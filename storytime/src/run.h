#pragma once

#include "new_application.h"

namespace Storytime {
    struct AppConfig {
        LogLevel log_level;
        uint32_t open_gl_version_major;
        uint32_t open_gl_version_minor;
        const char* glsl_version;
    };

    void run(const NewApplicationConfig& config);
}
