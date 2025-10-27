#pragma once

namespace Storytime {
    glm::vec2 to_vec2(const glm::vec3& vector);

    glm::vec2 to_vec2(const glm::vec4& vector);

    glm::vec3 to_vec3(const glm::vec2& vector);

    glm::vec3 to_vec3(const glm::vec4& vector);

    glm::vec4 to_vec4(const glm::vec2& vector);

    glm::vec4 to_vec4(const glm::vec3& vector);

    bool contains_nan(const glm::vec2& vector);

    bool contains_nan(const glm::vec3& vector);

    bool contains_nan(const glm::vec4& vector);

    bool contains_infinity(const glm::vec2& vector);

    bool contains_infinity(const glm::vec3& vector);

    bool contains_infinity(const glm::vec4& vector);

    bool contains_nan(const glm::mat4& matrix);

    bool contains_infinity(const glm::mat4& matrix);
}
