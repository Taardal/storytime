#pragma once

namespace Storytime {

    struct IDComponent {
        std::string uuid;
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
    };
}
