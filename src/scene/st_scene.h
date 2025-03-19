#pragma once

#include "st_entity.h"

#include <entt/entt.hpp>

namespace Storytime {
    class Scene {
    protected:
        entt::registry entity_registry;

    public:
        virtual ~Scene() = default;

        virtual void initialize() {}

        virtual void update(f64 timestep) = 0;

        virtual void render() = 0;

    protected:
        Entity create_entity();
    };
}
