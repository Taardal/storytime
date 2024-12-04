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
        T* get() const {
            Type type = get_type<T>();
            return static_cast<T*>(get(type));
        }

        void* get(Type type) const {
            auto it = services.find(type);
            if (it == services.end()) {
                ST_LOG_WARNING("Could not find service of type [{}]", type.name());
                return nullptr;
            }
            return it->second;
        }

        template<typename T>
        void set(void* service) {
            Type type = get_type<T>();
            set(type, service);
        }

        void set(Type type, void* service) {
            ST_ASSERT(service != nullptr, "Invalid service object: Cannot store nullptr");
            services.emplace(type, service);
        }

    private:
        template<typename T>
        static Type get_type() {
            return std::type_index(typeid(T));
        }
    };
}
