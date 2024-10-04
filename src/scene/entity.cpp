#include "entity.h"
#include "scene/components.h"

namespace Storytime {
    Entity::Entity(entt::entity entity, entt::registry* entity_registry)
        : entity(entity), entity_registry(entity_registry) {
        ST_ASSERT(entity != entt::null);
        ST_ASSERT(entity_registry != nullptr);
    }

    Entity::operator entt::entity() const {
        return entity;
    }

    Entity::operator u32() const {
        return (u32) entity;
    }

    Entity::operator bool() const {
        return entity != entt::null;
    }

    bool Entity::operator==(const Entity& other) const {
        return entity == other.entity && entity_registry == other.entity_registry;
    }

    bool Entity::operator!=(const Entity& other) const {
        return !(*this == other);
    }
}
