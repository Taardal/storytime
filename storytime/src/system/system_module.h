#pragma once

#include "storytime_config.h"
#include "file_system.h"

#include "module.h"

namespace Storytime {
    class SystemModule : public Module {
    private:
        FileSystem file_system;

    public:
        SystemModule();

        static void initialize(const Config& config);

        static void terminate();
    };
}
