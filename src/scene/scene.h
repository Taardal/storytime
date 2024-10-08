#pragma once

#include "graphics/renderer.h"
#include "scene/entity.h"

#include <entt/entt.hpp>

namespace Storytime {
    class Scene {
    protected:
        entt::registry entity_registry;

    public:
        virtual ~Scene() = default;

        virtual void on_initialize() {}

        virtual void on_update(f64 timestep) = 0;

        virtual void on_render(Renderer& renderer) = 0;

    protected:
        virtual void on_create_entities() = 0;

    protected:
        Entity create_entity();
    };
}
