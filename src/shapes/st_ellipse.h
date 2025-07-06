#pragma once

#include "st_intersect.h"
#include "tiled/st_tiled_object.h"

namespace Storytime {

    struct Point;
    struct Polygon;
    struct Rectangle;

    struct Ellipse {
        static u32 type;
        static std::string type_name;

        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 width = 0.0f;
        f32 height = 0.0f;

        static Ellipse create(const glm::vec2& position, const glm::vec2& size);

        static Ellipse create(const glm::vec3& position, const glm::vec3& size);

        f32 get_radius() const;

        f32 get_radius_x() const;

        f32 get_radius_y() const;

        f32 get_center_x() const;

        f32 get_center_y() const;

        bool intersects_with(const Ellipse& other) const;

        bool intersects_with(const Point& other) const;

        bool intersects_with(const Polygon& other) const;

        bool intersects_with(const Rectangle& other) const;
    };
}