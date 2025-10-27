#include "st_glm.h"

namespace Storytime {
    glm::vec2 to_vec2(const glm::vec3& vector) {
        return { vector.x, vector.y };
    }

    glm::vec2 to_vec2(const glm::vec4& vector) {
        return { vector.x, vector.y };
    }

    glm::vec3 to_vec3(const glm::vec2& vector) {
        return { vector.x, vector.y, 0.0f };
    }

    glm::vec3 to_vec3(const glm::vec4& vector) {
        return { vector.x, vector.y, vector.z };
    }

    glm::vec4 to_vec4(const glm::vec2& vector) {
        return { vector.x, vector.y, 0.0f, 0.0f };
    }

    glm::vec4 to_vec4(const glm::vec3& vector) {
        return { vector.x, vector.y, vector.z, 0.0f };
    }

    bool contains_nan(const glm::vec2& vector) {
        return glm::any(glm::isnan(vector));
    }

    bool contains_nan(const glm::vec3& vector) {
        return glm::any(glm::isnan(vector));
    }

    bool contains_nan(const glm::vec4& vector) {
        return glm::any(glm::isnan(vector));
    }

    bool contains_infinity(const glm::vec2& vector) {
        return glm::any(glm::isinf(vector));
    }

    bool contains_infinity(const glm::vec3& vector) {
        return glm::any(glm::isinf(vector));
    }

    bool contains_infinity(const glm::vec4& vector) {
        return glm::any(glm::isinf(vector));
    }

    bool contains_nan(const glm::mat4& matrix) {
        for (int i = 0; i < 4; i++) {
            const glm::vec4& vector = matrix[i];
            if (contains_nan(vector)) {
                return true;
            }
        }
        return false;
    }

    bool contains_infinity(const glm::mat4& matrix) {
        for (int i = 0; i < 4; i++) {
            const glm::vec4& vector = matrix[i];
            if (contains_infinity(vector)) {
                return true;
            }
        }
        return false;
    }
}
