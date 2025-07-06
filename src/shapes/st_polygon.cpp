#include "st_polygon.h"

namespace Storytime {
    u32 Polygon::type = 8433;
    std::string Polygon::type_name = "Polygon";

    Polygon Polygon::create(const glm::vec2& position, const std::vector<glm::vec2>& polygons) {
        return Polygon{
            .x = position.x,
            .y = position.y,
            .polygons = polygons,
        };
    }

    Polygon Polygon::create(const glm::vec3& position, const std::vector<glm::vec2>& polygons) {
        return Polygon{
            .x = position.x,
            .y = position.y,
            .polygons = polygons,
        };
    }

    bool Polygon::intersects_with(const Ellipse& other) const {
        return intersects(*this, other);
    }

    bool Polygon::intersects_with(const Point& other) const {
        return intersects(*this, other);
    }

    bool Polygon::intersects_with(const Polygon& other) const {
        return intersects(*this, other);
    }

    bool Polygon::intersects_with(const Rectangle& other) const {
        return intersects(*this, other);
    }
}
