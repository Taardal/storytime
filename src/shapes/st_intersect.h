#pragma once

namespace Storytime {

    constexpr float intersection_epsilon = 0.01f;

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

    bool intersects(const Rectangle& rectangle_a, const Rectangle& rectangle_b, f32 epsilon = intersection_epsilon);

    bool intersects_horizontally(const Rectangle& rectangle_a, const Rectangle& rectangle_b, f32 epsilon = intersection_epsilon);

    bool intersects_vertically(const Rectangle& rectangle_a, const Rectangle& rectangle_b, f32 epsilon = intersection_epsilon);

    bool intersects(
        const glm::vec4& rectangle_a,
        const glm::vec4& rectangle_b,
        f32 epsilon = intersection_epsilon
    );

    bool intersects_horizontally(
        const glm::vec4& rectangle_a,
        const glm::vec4& rectangle_b,
        f32 epsilon = intersection_epsilon
    );

    bool intersects_vertically(
        const glm::vec4& rectangle_a,
        const glm::vec4& rectangle_b,
        f32 epsilon = intersection_epsilon
    );

    bool intersects(
        const glm::vec3& position_a,
        const glm::vec3& size_a,
        const glm::vec3& position_b,
        const glm::vec3& size_b,
        f32 epsilon = intersection_epsilon
    );

    bool intersects_horizontally(
        const glm::vec3& position_a,
        const glm::vec3& size_a,
        const glm::vec3& position_b,
        const glm::vec3& size_b,
        f32 epsilon = intersection_epsilon
    );

    bool intersects_vertically(
        const glm::vec3& position_a,
        const glm::vec3& size_a,
        const glm::vec3& position_b,
        const glm::vec3& size_b,
        f32 epsilon = intersection_epsilon
    );

    bool intersects(
        f32 a_left,
        f32 a_right,
        f32 a_top,
        f32 a_bottom,
        f32 b_left,
        f32 b_right,
        f32 b_top,
        f32 b_bottom,
        f32 epsilon = intersection_epsilon
    );

    bool intersects_horizontally(
        f32 a_left,
        f32 a_right,
        f32 b_left,
        f32 b_right,
        f32 epsilon = intersection_epsilon
    );

    bool intersects_vertically(
        f32 a_top,
        f32 a_bottom,
        f32 b_top,
        f32 b_bottom,
        f32 epsilon = intersection_epsilon
    );

    struct RectangleIntersectConfig {
        f32 a_left = 0.0f;
        f32 a_right = 0.0f;
        f32 a_top = 0.0f;
        f32 a_bottom = 0.0f;
        f32 b_left = 0.0f;
        f32 b_right = 0.0f;
        f32 b_top = 0.0f;
        f32 b_bottom = 0.0f;
        f32 epsilon = intersection_epsilon;
    };

    bool intersects(const RectangleIntersectConfig& config);

    bool intersects_horizontally(const RectangleIntersectConfig& config);

    bool intersects_vertically(const RectangleIntersectConfig& config);
}
