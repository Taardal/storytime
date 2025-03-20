#pragma once

#include "system/st_service_locator.h"

namespace Storytime {
    class Module {
    protected:
        ServiceLocator service_locator;

    // protected:
    //     template<typename T>
    //     void set(void* service) {
    //         service_locator.set<T>(service);
    //     }

    public:
        template<typename T>
        T* get() {
            return service_locator.get<T>();
        }

        void* operator new(size_t size, const char* file_name, int line_number);

        void operator delete(void* pointer) noexcept;
    };
}
