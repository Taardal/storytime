#include "st_shape.h"
#include "st_intersect.h"

namespace Storytime {
    Shape::Shape(Ellipse ellipse) : Variant(std::move(ellipse)) {}

    Shape::Shape(Point point) : Variant(std::move(point)) {}

    Shape::Shape(Polygon polygon) : Variant(std::move(polygon)) {}

    Shape::Shape(Rectangle rectangle) : Variant(std::move(rectangle)) {}

    bool Shape::intersects_with(const Shape& other) const {
        return intersects(*this, other);
    }
}

namespace Storytime {
    bool intersects(const Shape& shape_a, const Shape& shape_b) {
        bool a_is_ellipse = shape_a.is<Ellipse>();
        bool a_is_point = shape_a.is<Point>();
        bool a_is_polygon = shape_a.is<Polygon>();
        bool a_is_rectangle = shape_a.is<Rectangle>();

        bool b_is_ellipse = shape_b.is<Ellipse>();
        bool b_is_point = shape_b.is<Point>();
        bool b_is_polygon = shape_b.is<Polygon>();
        bool b_is_rectangle = shape_b.is<Rectangle>();

        //
        // Ellipse
        //

        if (a_is_ellipse && b_is_ellipse) {
            return intersects(shape_a.as<Ellipse>(), shape_b.as<Ellipse>());
        }
        if (a_is_ellipse && b_is_point) {
            return intersects(shape_a.as<Ellipse>(), shape_b.as<Point>());
        }
        if (a_is_ellipse && b_is_polygon) {
            return intersects(shape_a.as<Ellipse>(), shape_b.as<Polygon>());
        }
        if (a_is_ellipse && b_is_rectangle) {
            return intersects(shape_a.as<Ellipse>(), shape_b.as<Rectangle>());
        }

        //
        // Point
        //

        if (a_is_point && b_is_ellipse) {
            return intersects(shape_a.as<Point>(), shape_b.as<Ellipse>());
        }
        if (a_is_point && b_is_point) {
            return intersects(shape_a.as<Point>(), shape_b.as<Point>());
        }
        if (a_is_point && b_is_polygon) {
            return intersects(shape_a.as<Point>(), shape_b.as<Polygon>());
        }
        if (a_is_point && b_is_rectangle) {
            return intersects(shape_a.as<Point>(), shape_b.as<Rectangle>());
        }

        //
        // Polygon
        //

        if (a_is_polygon && b_is_ellipse) {
            return intersects(shape_a.as<Polygon>(), shape_b.as<Ellipse>());
        }
        if (a_is_polygon && b_is_point) {
            return intersects(shape_a.as<Polygon>(), shape_b.as<Point>());
        }
        if (a_is_polygon && b_is_polygon) {
            return intersects(shape_a.as<Polygon>(), shape_b.as<Polygon>());
        }
        if (a_is_polygon && b_is_rectangle) {
            return intersects(shape_a.as<Polygon>(), shape_b.as<Rectangle>());
        }

        //
        // Rectangle
        //

        if (a_is_rectangle && b_is_ellipse) {
            return intersects(shape_a.as<Rectangle>(), shape_b.as<Ellipse>());
        }
        if (a_is_rectangle && b_is_point) {
            return intersects(shape_a.as<Rectangle>(), shape_b.as<Point>());
        }
        if (a_is_rectangle && b_is_polygon) {
            return intersects(shape_a.as<Rectangle>(), shape_b.as<Polygon>());
        }
        if (a_is_rectangle && b_is_rectangle) {
            return intersects(shape_a.as<Rectangle>(), shape_b.as<Rectangle>());
        }

        ST_LOG_WARNING("Could not resolve collider types to calculate collision");
        return false;
    }
}
