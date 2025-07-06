#include "st_point.h"

namespace Storytime {
    u32 Point::type = 8432;
    std::string Point::type_name = "Point";

    Point Point::create(const glm::vec2& position) {
        return Point{
            .x = position.x,
            .y = position.y,
        };
    }

    Point Point::create(const glm::vec3& position) {
        return Point{
            .x = position.x,
            .y = position.y,
        };
    }

    Point::operator glm::vec2() const {
        return glm::vec2{ x, y, };
    }

    Point::operator glm::vec3() const {
        return glm::vec3{ x, y, 0.0f };
    }

    bool Point::intersects_with(const Ellipse& other) const {
        return intersects(*this, other);
    }

    bool Point::intersects_with(const Point& other) const {
        return intersects(*this, other);
    }

    bool Point::intersects_with(const Polygon& other) const {
        return intersects(*this, other);
    }

    bool Point::intersects_with(const Rectangle& other) const {
        return intersects(*this, other);
    }
}
