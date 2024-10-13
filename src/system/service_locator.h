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
            Type type = get_type<T>();
            auto it = services.find(type);
            if (it == services.end()) {
                ST_LOG_WARNING("Could not find service of type [{}]", type.name());
                return nullptr;
            }
            return static_cast<T*>(it->second);
        }

        template<typename T>
        void set(void* service) {
            ST_ASSERT(service != nullptr, "Invalid service object: Cannot store nullptr");
            Type type = get_type<T>();
            services.emplace(type, service);
        }

    private:
        template<typename T>
        static Type get_type() {
            return std::type_index(typeid(T));
        }
    };
}
