#include "st_intersect.h"
#include "st_ellipse.h"
#include "st_point.h"
#include "st_polygon.h"
#include "st_rectangle.h"
#include "st_shape.h"

namespace Storytime {

    constexpr float epsilon = 0.01f;

    // ------------------------------------------------------------------------------------
    // Ellipse
    // ------------------------------------------------------------------------------------

    bool intersects(const Ellipse& ellipse_a, const Ellipse& ellipse_b) {
        f32 distance_center_x = ellipse_a.x - ellipse_b.x;
        f32 distance_center_y = ellipse_a.y - ellipse_b.y;
        f32 distance_between_centers = std::sqrt(distance_center_x * distance_center_x + distance_center_y * distance_center_y);
        return distance_between_centers < ellipse_a.get_radius() + ellipse_b.get_radius();
    }

    bool intersects(const Ellipse& ellipse, const Point& point) {
        f32 ellipse_radius_x = ellipse.get_radius_x();
        f32 ellipse_radius_y = ellipse.get_radius_y();
        f32 ellipse_center_x = ellipse.x + ellipse_radius_x;
        f32 ellipse_center_y = ellipse.y + ellipse_radius_y;

        f32 normalized_distance_x = (point.x - ellipse_center_x) / ellipse_radius_x;
        f32 normalized_distance_y = (point.y - ellipse_center_y) / ellipse_radius_y;
        f32 normalized_distance = normalized_distance_x * normalized_distance_x + normalized_distance_y * normalized_distance_y;

        return normalized_distance <= 1.0f;
    }

    bool intersects(const Ellipse& ellipse, const Rectangle& rectangle) {
        f32 ellipse_radius_x = ellipse.width * 0.5f;
        f32 ellipse_radius_y = ellipse.height * 0.5f;
        f32 ellipse_center_x = ellipse.x + ellipse_radius_x;
        f32 ellipse_center_y = ellipse.y + ellipse_radius_y;

        // Find the point on the rectangle that is closest to the center of the ellipse
        f32 closest_x = std::max(rectangle.x, std::min(ellipse_center_x, rectangle.x + rectangle.width));
        f32 closest_y = std::max(rectangle.y, std::min(ellipse_center_y, rectangle.y + rectangle.height));

        // Use ellipse equation to check if the point closest to the center is inside the ellipse
        f32 normalized_distance_x = (closest_x - ellipse_center_x) / ellipse_radius_x;
        f32 normalized_distance_y = (closest_y - ellipse_center_y) / ellipse_radius_y;
        f32 normalized_distance = normalized_distance_x * normalized_distance_x + normalized_distance_y * normalized_distance_y;

        return normalized_distance <= 1.0f;
    }

    bool intersects(const Ellipse& ellipse, const Polygon& polygon) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    // ------------------------------------------------------------------------------------
    // Point
    // ------------------------------------------------------------------------------------

    bool intersects(const Point& point, const Ellipse& ellipse) {
        return intersects(ellipse, point);
    }

    bool intersects(const Point& point_a, const Point& point_b) {
        return point_a.x == point_b.x && point_a.y == point_b.y;
    }

    bool intersects(const Point& point, const Polygon& polygon) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    bool intersects(const Point& point, const Rectangle& rectangle) {
        bool point_inside_rectangle_x = point.x > rectangle.get_left() - epsilon && point.x < rectangle.get_right() + epsilon;
        bool point_inside_rectangle_y = point.y > rectangle.get_top() - epsilon && point.y < rectangle.get_bottom() + epsilon;
        return point_inside_rectangle_x && point_inside_rectangle_y;
    }

    // ------------------------------------------------------------------------------------
    // Polygon
    // ------------------------------------------------------------------------------------

    bool intersects(const Polygon& polygon, const Ellipse& ellipse) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    bool intersects(const Polygon& polygon, const Point& point) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    bool intersects(const Polygon& polygon_a, const Polygon& polygon_b) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    bool intersects(const Polygon& polygon, const Rectangle& rectangle) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    // ------------------------------------------------------------------------------------
    // Rectangle
    // ------------------------------------------------------------------------------------

    bool intersects(const Rectangle& rectangle, const Ellipse& ellipse) {
        return intersects(ellipse, rectangle);
    }

    bool intersects(const Rectangle& rectangle, const Point& point) {
        return intersects(point, rectangle);
    }

    bool intersects(const Rectangle& rectangle, const Polygon& polygon) {
        ST_ASSERT(false, "Not implemented");
        return false;
    }

    bool intersects(const Rectangle& rectangle_a, const Rectangle& rectangle_b) {
        f32 a_left = rectangle_a.get_left();
        f32 a_right = rectangle_a.get_right();
        f32 a_top = rectangle_a.get_top();
        f32 a_bottom = rectangle_a.get_bottom();

        f32 b_left = rectangle_b.get_left();
        f32 b_right = rectangle_b.get_right();
        f32 b_top = rectangle_b.get_top();
        f32 b_bottom = rectangle_b.get_bottom();

        bool intersects_horizontally = a_left < b_right - epsilon && a_right > b_left + epsilon;
        bool intersects_vertically = a_top < b_bottom - epsilon && a_bottom > b_top + epsilon;

        return intersects_horizontally && intersects_vertically;
    }
}
