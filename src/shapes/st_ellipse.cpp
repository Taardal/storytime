#include "st_ellipse.h"

namespace Storytime {
    u32 Ellipse::type = 8431;
    std::string Ellipse::type_name = "Ellipse";

    Ellipse Ellipse::create(const glm::vec2& position, const glm::vec2& size) {
        return Ellipse{
            .x = position.x,
            .y = position.y,
            .width = size.x,
            .height = size.y,
        };
    }

    Ellipse Ellipse::create(const glm::vec3& position, const glm::vec3& size) {
        return Ellipse{
            .x = position.x,
            .y = position.y,
            .width = size.x,
            .height = size.y,
        };
    }

    f32 Ellipse::get_radius() const {
        return std::max(get_radius_x(), get_radius_y());
    }

    f32 Ellipse::get_radius_x() const {
        return width * 0.5f;
    }

    f32 Ellipse::get_radius_y() const {
        return height * 0.5f;
    }

    f32 Ellipse::get_center_x() const {
        return x + get_radius_x();
    }

    f32 Ellipse::get_center_y() const {
        return y + get_radius_y();
    }

    bool Ellipse::intersects_with(const Ellipse& other) const {
        return intersects(*this, other);
    }

    bool Ellipse::intersects_with(const Point& other) const {
        return intersects(*this, other);
    }

    bool Ellipse::intersects_with(const Polygon& other) const {
        return intersects(*this, other);
    }

    bool Ellipse::intersects_with(const Rectangle& other) const {
        return intersects(*this, other);
    }
}
