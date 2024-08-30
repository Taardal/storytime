#pragma once

#include "system/service_locator.h"

namespace Storytime {
    struct Module {
        void* operator new(size_t size, const char* file_name, int line_number);

        void operator delete(void* pointer) noexcept;

        // ServiceLocator* service_locator;
        //
        // template<typename T>
        // T* get() {
        //     return service_locator.get<T>();
        // }
    };
}
