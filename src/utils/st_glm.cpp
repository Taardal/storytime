#include "st_glm.h"

namespace Storytime {
    glm::vec2 to_vec2(const glm::vec3& vec) {
        return { vec.x, vec.y };
    }

    glm::vec2 to_vec2(const glm::vec4& vec) {
        return { vec.x, vec.y };
    }

    glm::vec3 to_vec3(const glm::vec2& vec) {
        return { vec.x, vec.y, 0.0f };
    }

    glm::vec3 to_vec3(const glm::vec4& vec) {
        return { vec.x, vec.y, vec.z };
    }

    glm::vec4 to_vec4(const glm::vec2& vec) {
        return { vec.x, vec.y, 0.0f, 0.0f };
    }

    glm::vec4 to_vec4(const glm::vec3& vec) {
        return { vec.x, vec.y, vec.z, 0.0f };
    }
}
