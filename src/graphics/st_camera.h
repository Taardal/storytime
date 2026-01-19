#pragma once

#include "st_view_projection.h"

namespace Storytime {
    class Camera {
    protected:
        ViewProjection view_projection{};

    public:
        virtual ~Camera() = default;

        const ViewProjection& get_view_projection() const;

        const glm::mat4& get_view() const;

        const glm::mat4& get_projection() const;

        virtual void update() = 0;
    };
}
