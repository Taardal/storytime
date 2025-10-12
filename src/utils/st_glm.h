#pragma once

namespace Storytime {
    glm::vec2 to_vec2(const glm::vec3& vec3);

    glm::vec2 to_vec2(const glm::vec4& vec4);

    glm::vec3 to_vec3(const glm::vec2& vec2);

    glm::vec3 to_vec3(const glm::vec4& vec4);

    glm::vec4 to_vec4(const glm::vec2& vec2);

    glm::vec4 to_vec4(const glm::vec3& vec3);
}
