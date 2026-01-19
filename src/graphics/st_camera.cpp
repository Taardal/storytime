#include "st_camera.h"

namespace Storytime {
    const ViewProjection& Camera::get_view_projection() const {
        return view_projection;
    }

    const glm::mat4& Camera::get_view() const {
        return view_projection.view;
    }

    const glm::mat4& Camera::get_projection() const {
        return view_projection.projection;
    }
}
