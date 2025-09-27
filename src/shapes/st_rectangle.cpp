#include "st_rectangle.h"

namespace Storytime {
    u32 Rectangle::type = 8434;
    std::string Rectangle::type_name = "Rectangle";

    Rectangle Rectangle::create(const glm::vec2& position, const glm::vec2& size) {
        return Rectangle{
            .x = position.x,
            .y = position.y,
            .width = size.x,
            .height = size.y,
        };
    }

    Rectangle Rectangle::create(const glm::vec3& position, const glm::vec3& size) {
        return Rectangle{
            .x = position.x,
            .y = position.y,
            .width = size.x,
            .height = size.y,
        };
    }

    bool Rectangle::is_valid() const {
        return width > 0 && height > 0;
    }

    void Rectangle::invalidate() {
        width = 0;
        height = 0;
    }

    f32 Rectangle::get_left() const {
        return x;
    }

    f32 Rectangle::get_right() const {
        return x + width;
    }

    f32 Rectangle::get_top() const {
        return y;
    }

    f32 Rectangle::get_bottom() const {
        return y + height;
    }

    bool Rectangle::intersects_with(const Ellipse& other) const {
        return intersects(*this, other);
    }

    bool Rectangle::intersects_with(const Point& other) const {
        return intersects(*this, other);
    }

    bool Rectangle::intersects_with(const Polygon& other) const {
        return intersects(*this, other);
    }

    bool Rectangle::intersects_with(const Rectangle& other, f32 epsilon) const {
        return intersects(*this, other);
    }

    bool Rectangle::intersects_horizontally_with(const Rectangle& other, f32 epsilon) const {
        return intersects_horizontally(*this, other);
    }

    bool Rectangle::intersects_vertically_with(const Rectangle& other, f32 epsilon) const {
        return intersects_vertically(*this, other);
    }
}
