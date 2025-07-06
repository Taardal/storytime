#pragma once

#include "st_intersect.h"

namespace Storytime {

    struct Ellipse;
    struct Point;
    struct Rectangle;

    struct Polygon {
        static u32 type;
        static std::string type_name;

        f32 x = 0.0f;
        f32 y = 0.0f;
        std::vector<glm::vec2> polygons;

        static Polygon create(const glm::vec2& position, const std::vector<glm::vec2>& polygons);

        static Polygon create(const glm::vec3& position, const std::vector<glm::vec2>& polygons);

        bool intersects_with(const Ellipse& other) const;

        bool intersects_with(const Point& other) const;

        bool intersects_with(const Polygon& other) const;

        bool intersects_with(const Rectangle& other) const;
    };
}