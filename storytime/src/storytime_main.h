#pragma once

#include "storytime_config.h"
#include "storytime_app.h"

namespace Storytime {
    void run(const Config& config, const std::function<App*(const Config&)>& create_app);

    template<typename T>
    void run(const Config& config) {
        run(config, [](const Config& cfg) {
            return ST_NEW T(cfg);
        });
    }
}
