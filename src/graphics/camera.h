#pragma once

#include <glm/glm.hpp>

namespace Storytime {
    struct ProjectionSettings {
        f32 left = 0.0f;
        f32 right = 0.0f;
        f32 bottom = 0.0f;
        f32 top = 0.0f;
        f32 z_near = -1.0f;
        f32 z_far = 1.0f;
    };

    class Camera {
    private:
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view_projection = glm::mat4(1.0f);
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        f32 rotation = 0.0f;

    public:
        glm::mat4 get_view_projection() const;

        void set_projection(const ProjectionSettings& projection_settings);

        void set_position(const glm::vec3& position);

        void set_rotation(f32 rotation);

    private:
        void calculate();
    };
}
