#pragma once

#include "st_view_projection.h"

namespace Storytime {
    struct OrthographicProjection {
        f32 left = 0.0f;
        f32 right = 0.0f;
        f32 bottom = 0.0f;
        f32 top = 0.0f;
        f32 z_near = -1.0f;
        f32 z_far = 1.0f;
    };

    struct CameraConfig {
        OrthographicProjection orthographic_projection{};
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        f32 rotation = 0.0f;
        f32 zoom = 0.0f;
        f32 aspect_ratio = 0.0f;
    };

    class Camera {
    public:
        CameraConfig config;
        ViewProjection view_projection{};

    public:
        explicit Camera(const CameraConfig& config);

        ViewProjection get_view_projection() const;

        void set_projection(const OrthographicProjection& orthographic_projection);

        void set_position(const glm::vec3& position);

        void move_x(f32 x_offset);

        void move_y(f32 y_offset);

        void set_rotation(f32 rotation);

        void set_aspect_ratio(f32 aspect_ratio);

        void resize(f32 width, f32 height);

        void set_zoom(f32 zoom);

        void zoom_by(f32 offset);

    private:
        void calculate();
    };
}
