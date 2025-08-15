#pragma once

#include "st_intersect.h"

namespace Storytime {

    struct Ellipse;
    struct Point;
    struct Polygon;

    struct Rectangle {
        static u32 type;
        static std::string type_name;

        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 width = 0.0f;
        f32 height = 0.0f;

        static Rectangle create(const glm::vec2& position, const glm::vec2& size);

        static Rectangle create(const glm::vec3& position, const glm::vec3& size);

        bool is_valid() const;

        f32 get_left() const;

        f32 get_right() const;

        f32 get_top() const;

        f32 get_bottom() const;

        bool intersects_with(const Ellipse& other) const;

        bool intersects_with(const Point& other) const;

        bool intersects_with(const Polygon& other) const;

        bool intersects_with(const Rectangle& other) const;

        bool intersects_horizontally_with(const Rectangle& other) const;

        bool intersects_vertically_with(const Rectangle& other) const;
    };
}