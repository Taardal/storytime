#include "st_intersect.h"
#include "st_ellipse.h"
#include "st_point.h"
#include "st_polygon.h"
#include "st_rectangle.h"
#include "st_shape.h"

namespace Storytime {

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
        bool point_inside_rectangle_x = point.x > rectangle.get_left() - intersection_epsilon && point.x < rectangle.get_right() + intersection_epsilon;
        bool point_inside_rectangle_y = point.y > rectangle.get_top() - intersection_epsilon && point.y < rectangle.get_bottom() + intersection_epsilon;
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

    bool intersects(const Rectangle& rectangle_a, const Rectangle& rectangle_b, f32 epsilon) {
        if (!intersects_horizontally(rectangle_a, rectangle_b, epsilon)) {
            return false;
        }
        if (!intersects_vertically(rectangle_a, rectangle_b, epsilon)) {
            return false;
        }
        return true;
    }

    bool intersects_horizontally(const Rectangle& rectangle_a, const Rectangle& rectangle_b, f32 epsilon) {
        if (rectangle_a.get_left() >= rectangle_b.get_right() - epsilon) {
            return false;
        }
        if (rectangle_a.get_right() <= rectangle_b.get_left() + epsilon) {
            return false;
        }
        return true;
    }

    bool intersects_vertically(const Rectangle& rectangle_a, const Rectangle& rectangle_b, f32 epsilon) {
        if (rectangle_a.get_top() >= rectangle_b.get_bottom() - epsilon) {
            return false;
        }
        if (rectangle_a.get_bottom() <= rectangle_b.get_top() + epsilon) {
            return false;
        }
        return true;
    }

    bool intersects(const glm::vec4& rectangle_a, const glm::vec4& rectangle_b, f32 epsilon) {
        f32 a_left = rectangle_a.x;
        f32 a_right = rectangle_a.x + rectangle_a.z;
        f32 a_top = rectangle_a.y;
        f32 a_bottom = rectangle_a.y + rectangle_a.w;

        f32 b_left = rectangle_b.x;
        f32 b_right = rectangle_b.x + rectangle_b.z;
        f32 b_top = rectangle_b.y;
        f32 b_bottom = rectangle_b.y + rectangle_b.w;

        return intersects(
            a_left,
            a_right,
            a_top,
            a_bottom,
            b_left,
            b_right,
            b_top,
            b_bottom,
            epsilon
        );
    }

    bool intersects(const glm::vec3& position_a, const glm::vec3& size_a, const glm::vec3& position_b, const glm::vec3& size_b, f32 epsilon) {
        f32 a_left = position_a.x;
        f32 a_right = position_a.x + size_a.x;
        f32 a_top = position_a.y;
        f32 a_bottom = position_a.y + size_a.y;

        f32 b_left = position_b.x;
        f32 b_right = position_b.x + size_b.x;
        f32 b_top = position_b.y;
        f32 b_bottom = position_b.y + size_b.y;

        return intersects(
            a_left,
            a_right,
            a_top,
            a_bottom,
            b_left,
            b_right,
            b_top,
            b_bottom,
            epsilon
        );
    }

    bool intersects(
        f32 a_left,
        f32 a_right,
        f32 a_top,
        f32 a_bottom,
        f32 b_left,
        f32 b_right,
        f32 b_top,
        f32 b_bottom,
        f32 epsilon
    ) {
        if (!intersects_horizontally(a_left, a_right, b_left, b_right, epsilon)) {
            return false;
        }
        if (!intersects_vertically(a_top, a_bottom, b_top, b_bottom, epsilon)) {
            return false;
        }
        return true;
    }

    bool intersects_horizontally(
        f32 a_left,
        f32 a_right,
        f32 b_left,
        f32 b_right,
        f32 epsilon
    ) {
        if (a_left >= b_right - epsilon) {
            return false;
        }
        if (a_right <= b_left + epsilon) {
            return false;
        }
        return true;
    }

    bool intersects_vertically(
        f32 a_top,
        f32 a_bottom,
        f32 b_top,
        f32 b_bottom,
        f32 epsilon
    ) {
        if (a_top >= b_bottom - epsilon) {
            return false;
        }
        if (a_bottom <= b_top + epsilon) {
            return false;
        }
        return true;
    }

    bool intersects(const RectangleIntersectConfig& config) {
        if (!intersects_horizontally(config)) {
            return false;
        }
        if (!intersects_vertically(config)) {
            return false;
        }
        return true;
    }

    bool intersects_horizontally(const RectangleIntersectConfig& config) {
        if (config.a_left >= config.b_right - config.epsilon) {
            return false;
        }
        if (config.a_right <= config.b_left + config.epsilon) {
            return false;
        }
        return true;
    }

    bool intersects_vertically(const RectangleIntersectConfig& config) {
        if (config.a_top >= config.b_bottom - config.epsilon) {
            return false;
        }
        if (config.a_bottom <= config.b_top + config.epsilon) {
            return false;
        }
        return true;
    }
}
