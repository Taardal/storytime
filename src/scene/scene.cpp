#include "scene.h"
#include "scene/components.h"
#include "system/uuid.h"

namespace Storytime {
    Entity Scene::create_entity() {
        entt::entity entity = entity_registry.create();

        IDComponent id_component{};
        id_component.uuid = UUID::create();
        entity_registry.emplace<IDComponent>(entity, id_component);

        TagComponent tag_component{};
        tag_component.tag = "Entity " + std::to_string((u32) entity);
        entity_registry.emplace<TagComponent>(entity, tag_component);

        TransformComponent transform_component{};
        transform_component.position = { 0.0f, 0.0f, 0.0f };
        entity_registry.emplace<TransformComponent>(entity, transform_component);

        return Entity(entity, &entity_registry);
    }
}
