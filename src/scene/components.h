#pragma once

#include "direction.h"

namespace Storytime {

    struct IDComponent {
        std::string uuid;
    };

    struct SpriteComponent {
        Sprite sprite;
    };

    struct TagComponent {
        std::string tag;
    };

    struct TransformComponent {
        glm::mat4 translation = glm::mat4(1.0f);
        glm::mat4 rotation = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);

        Direction direction_x = Direction::None;
        Direction direction_y = Direction::None;
        f32 velocity_x = 0.0f;
        f32 velocity_y = 0.0f;
        f32 move_speed = 10.0f;
        bool collision = false;
    };
}
