#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Storytime {
    glm::mat4 Camera::get_view_projection() const {
        return view_projection;
    }

    void Camera::set_projection(const ProjectionSettings& projection_settings) {
        projection = glm::ortho(
            projection_settings.left,
            projection_settings.right,
            projection_settings.bottom,
            projection_settings.top,
            projection_settings.z_near,
            projection_settings.z_far
        );
        calculate();
    }

    void Camera::set_position(const glm::vec3& position) {
        this->position = position;
        calculate();
    }

    void Camera::set_rotation(f32 rotation) {
        this->rotation = rotation;
        calculate();
    }

    void Camera::calculate() {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

        f32 angle = glm::radians(rotation);
        static auto z_axis = glm::vec3(0, 0, 1);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, z_axis);

        view = translation * rotation;
        view_projection = projection * view;
    }
}
