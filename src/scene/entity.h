#pragma once

#include "scene/components.h"
#include <entt/entt.hpp>

namespace Storytime {
    class Entity {
    private:
        entt::entity entity = entt::null;
        entt::registry* entity_registry = nullptr;

    public:
        Entity() = default;

        Entity(const Entity& other) = default;

        Entity(entt::entity entity, entt::registry* entity_registry);

        operator entt::entity() const;

        operator u32() const;

        operator bool() const;

        bool operator==(const Entity& other) const;

        bool operator!=(const Entity& other) const;

        template<typename T>
        T get_component() {
            ST_ASSERT(entity != entt::null);
            ST_ASSERT(entity_registry != nullptr);
            return entity_registry->get<T>(entity);
        }

        template<typename T, typename... Args>
        void add_component(Args&&... args) {
            ST_ASSERT(entity != entt::null);
            ST_ASSERT(entity_registry != nullptr);
            return entity_registry->emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        bool has_component() const {
            ST_ASSERT(entity != entt::null);
            ST_ASSERT(entity_registry != nullptr);
            return entity_registry->try_get<T>(entity) != entt::null;
        }

        template<typename T>
        void remove_component() const {
            ST_ASSERT(has_component<T>());
            entity_registry->remove<T>(entity);
        }

        // const std::string& get_uuid();
        //
        // const std::string& get_name();
    };
}
