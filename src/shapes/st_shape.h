#pragma once

#include "st_ellipse.h"
#include "st_point.h"
#include "st_polygon.h"
#include "st_rectangle.h"
#include "st_intersect.h"

#include "system/st_variant.h"

namespace Storytime {
    class Shape : public Variant<Ellipse, Point, Polygon, Rectangle> {
    public:
        Shape(Ellipse ellipse);

        Shape(Point point);

        Shape(Polygon polygon);

        Shape(Rectangle rectangle);

        bool intersects_with(const Shape& other) const;
    };

    bool intersects(const Shape& shape_a, const Shape& shape_b);

    template<typename A, typename B>
    bool intersects(const Shape& shape_a, const Shape& shape_b) {
        ST_ASSERT(shape_a.is<A>(), "Shape a must be of type [" << type_name<A>() << "]");
        const A& a = shape_a.as<A>();

        ST_ASSERT(shape_b.is<B>(), "Shape b must be of type [" << type_name<B>() << "]");
        const B& b = shape_b.as<B>();

        return intersects(a, b);
    }
}
