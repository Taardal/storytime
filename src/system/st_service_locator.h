#pragma once

#include "system/st_assert.h"

#include <typeindex>

namespace Storytime {
    class ServiceLocator {
    private:
        std::map<std::type_index, void*> services;

    public:
        template<typename T>
        T& get() const {
            T* service = try_get<T>();
            ST_ASSERT(service != nullptr, "Service [" << type_name<T>() << "] must exist");
            return *service;
        }

        template<typename T>
        T* try_get() const {
            auto type = std::type_index(typeid(T));
            auto it = services.find(type);
            if (it == services.end()) {
                ST_LOG_WARNING("Could not find service of type [{}]", demangle(type.name()));
                return nullptr;
            }
            return static_cast<T*>(it->second);
        }

        template<typename T>
        void set(void* service) {
            ST_ASSERT(service != nullptr, "Invalid service object: Cannot store nullptr");
            auto type = std::type_index(typeid(T));
            services.emplace(type, service);
        }
    };
}
