#pragma once

#include <entt/entt.hpp>

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_VALID_ENTITY() \
        ST_ASSERT(entity != entt::null, "Invalid entity"); \
        ST_ASSERT(entity_registry != nullptr, "Invalid entity registry")
#else
    #define ST_ASSERT_VALID_ENTITY()
#endif

namespace Storytime {
    class Entity {
    protected:
        entt::entity entity = entt::null;
        entt::registry* entity_registry = nullptr;

    public:
        Entity() = default;

        Entity(const Entity& other) = default;

        Entity(entt::entity entity, entt::registry* entity_registry);

        entt::entity raw() const;

        operator entt::entity() const;

        operator u32() const;

        operator bool() const;

        bool operator==(const Entity& other) const;

        bool operator!=(const Entity& other) const;

        template<typename T>
        T& get() {
            ST_ASSERT_VALID_ENTITY();
            ST_ASSERT(has<T>(), "Entity [" << (u32) entity << "] must have component [" << type_name<T>() << "]");
            return entity_registry->get<T>(entity);
        }

        template<typename T, typename... Args>
        T& get_or_add(Args &&...args) {
            ST_ASSERT_VALID_ENTITY();
            return entity_registry->get_or_emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        T* try_get() {
            ST_ASSERT_VALID_ENTITY();
            return entity_registry->try_get<T>(entity);
        }

        template<typename T, typename... Args>
        void set(Args&&... args) {
            ST_ASSERT_VALID_ENTITY();
            entity_registry->emplace_or_replace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        void add(Args&&... args) {
            ST_ASSERT_VALID_ENTITY();
            ST_ASSERT(!has<T>(), "Cannot add component that the entity already have");
            entity_registry->emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        bool has() const {
            ST_ASSERT_VALID_ENTITY();
            return entity_registry->try_get<T>(entity) != nullptr;
        }

        template<typename T>
        void remove() const {
            ST_ASSERT_VALID_ENTITY();
            ST_ASSERT(has<T>(), "Cannot remove component that the entity does not have");
            entity_registry->remove<T>(entity);
        }
    };
}
