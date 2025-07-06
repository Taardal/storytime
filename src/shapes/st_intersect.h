#pragma once

namespace Storytime {

    struct Ellipse;
    struct Point;
    struct Polygon;
    struct Rectangle;

    // ------------------------------------------------------------------------------------
    // Ellipse
    // ------------------------------------------------------------------------------------

    bool intersects(const Ellipse& ellipse_a, const Ellipse& ellipse_b);

    bool intersects(const Ellipse& ellipse, const Point& point);

    bool intersects(const Ellipse& ellipse, const Rectangle& rectangle);

    bool intersects(const Ellipse& ellipse, const Polygon& polygon);

    // ------------------------------------------------------------------------------------
    // Point
    // ------------------------------------------------------------------------------------

    bool intersects(const Point& point, const Ellipse& ellipse);

    bool intersects(const Point& point_a, const Point& point_b);

    bool intersects(const Point& point, const Polygon& polygon);

    bool intersects(const Point& point, const Rectangle& rectangle);

    // ------------------------------------------------------------------------------------
    // Polygon
    // ------------------------------------------------------------------------------------

    bool intersects(const Polygon& polygon, const Ellipse& ellipse);

    bool intersects(const Polygon& polygon, const Point& point);

    bool intersects(const Polygon& polygon_a, const Polygon& polygon_b);

    bool intersects(const Polygon& polygon, const Rectangle& rectangle);

    // ------------------------------------------------------------------------------------
    // Rectangle
    // ------------------------------------------------------------------------------------

    bool intersects(const Rectangle& rectangle, const Ellipse& ellipse);

    bool intersects(const Rectangle& rectangle, const Point& point);

    bool intersects(const Rectangle& rectangle, const Polygon& polygon);

    bool intersects(const Rectangle& rectangle_a, const Rectangle& rectangle_b);
}
