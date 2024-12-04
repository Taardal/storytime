#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Storytime {
    Camera::Camera(const CameraConfig& config) : config(config) {
        set_projection(config.orthographic_projection);
    }

    ViewProjection Camera::get_view_projection() const {
        return view_projection;
    }

    void Camera::set_projection(const OrthographicProjection& orthographic_projection) {
        config.orthographic_projection = orthographic_projection;
        view_projection.projection = glm::ortho(
            config.orthographic_projection.left,
            config.orthographic_projection.right,
            config.orthographic_projection.bottom,
            config.orthographic_projection.top,
            config.orthographic_projection.z_near,
            config.orthographic_projection.z_far
        );
        calculate();
    }

    void Camera::set_position(const glm::vec3& position) {
        config.position = position;
        calculate();
    }

    void Camera::move_x(f32 x_offset) {
        set_position({ config.position.x + x_offset, config.position.y, 0.0f });
    }

    void Camera::move_y(f32 y_offset) {
        set_position({ config.position.x, config.position.y + y_offset, 0.0f });
    }

    void Camera::set_rotation(f32 rotation) {
        config.rotation = rotation;
        calculate();
    }

    void Camera::set_aspect_ratio(f32 aspect_ratio) {
        config.aspect_ratio = aspect_ratio;
        calculate();
    }

    void Camera::resize(f32 width, f32 height) {
        config.aspect_ratio = width / height;
        calculate();
    }

    void Camera::set_zoom(f32 zoom) {
        f32 offset = zoom - config.zoom;
        zoom_by(offset);
    }

    void Camera::zoom_by(f32 offset) {
        config.zoom -= offset;
        config.orthographic_projection.top += offset;
        config.orthographic_projection.bottom -= offset;
        config.orthographic_projection.left += offset * config.aspect_ratio;
        config.orthographic_projection.right -= offset * config.aspect_ratio;
        calculate();
    }

    void Camera::calculate() {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), config.position);

        f32 angle = glm::radians(config.rotation);
        static auto z_axis = glm::vec3(0, 0, 1);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, z_axis);

        view_projection.view = translation * rotation;
    }
}
