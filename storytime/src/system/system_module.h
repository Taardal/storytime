#pragma once

#include "storytime_config.h"
#include "service_locator.h"
#include "file_system.h"

#include "module.h"

namespace Storytime {
    struct SystemModule : public Module {
        ServiceLocator service_locator;
        FileSystem file_system;

        SystemModule();

        static void initialize(const Config& config);

        template<typename T>
        T* get() {
            return service_locator.get<T>();
        }
    };
}
