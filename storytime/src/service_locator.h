#pragma once

#include "system/assert.h"

#include <typeindex>

namespace Storytime {
    class ServiceLocator {
    private:
        typedef std::type_index Type;
    
    private:
        std::map<Type, void*> services;

    public:
        template<typename T>
        T* get() {
            Type type = std::type_index(typeid(T));
            auto it = services.find(type);
            ST_ASSERT(it != services.end());
            // if (it == services.end()) {
            //     return nullptr;
            // }
            return static_cast<T*>(it->second);
        }

        template<typename T>
        void set(void* service) {
            Type type = std::type_index(typeid(T));
            ST_ASSERT(service != nullptr);
            services.emplace(type, service);
        }
    };
}
