#include "st_scene.h"

namespace Storytime {
    Entity Scene::create_entity() {
        entt::entity entity = entity_registry.create();
        return Entity(entity, entity_registry);
    }
}
