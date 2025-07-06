#pragma once

#include "st_intersect.h"

namespace Storytime {

    struct Ellipse;
    struct Polygon;
    struct Rectangle;

    struct Point {
        static u32 type;
        static std::string type_name;

        f32 x = 0.0f;
        f32 y = 0.0f;

        static Point create(const glm::vec2& position);

        static Point create(const glm::vec3& position);

        operator glm::vec2() const;

        operator glm::vec3() const;

        bool intersects_with(const Ellipse& other) const;

        bool intersects_with(const Point& other) const;

        bool intersects_with(const Polygon& other) const;

        bool intersects_with(const Rectangle& other) const;
    };
}