#include "physics.hpp"
#include "p8g.hpp"
#include <utility>
#include <vector>
#include <cmath>

namespace Physics {
    std::pair<bool, Point2D> PointCircleCollision (Point2D point, Circle2D circle) {
        bool ret = false;
        Point2D ret_point{ 0, 0 };
        float distX = point.x - circle.center.x;
        float distY = point.y - circle.center.y;
        float distance = sqrt((distX * distX) + (distY * distY));

        if(distance <= circle.r) {
            float theta = atan2f(point.y - circle.center.y, point.x - circle.center.x);
            float x_cr = circle.center.x + circle.r * cos(theta);
            float y_cr = circle.center.y + circle.r * sin(theta);
            ret_point.x = static_cast<int32_t>(point.x - x_cr);
            ret_point.y = static_cast<int32_t>(point.y - y_cr);
            ret = true;
        }
        return std::make_pair(ret, ret_point);
    }

    bool PointLineCollision (Point2D point, Line2D line) {
        const float delta = 0.1;
        bool ret = false;
        float d1 = Line2D{ line.a, point }.Length();
        float d2 = Line2D{ line.b, point }.Length();
        float line_len = line.Length();

        if(d1 + d2 >= line_len - delta && d1 + d2 <= line_len + delta) {
            ret = true;
        }
        return ret;
    }

    std::pair<bool, Point2D> LineCircleCollision (Line2D line, Circle2D circle) {
        bool ret = false;
        Point2D ret_point{ 0, 0 };

        auto [ret_1, ret_point_1] = PointCircleCollision(line.a, circle);
        auto [ret_2, ret_point_2] = PointCircleCollision(line.b, circle);

        float dot = (((circle.center.x - line.a.x) * (line.b.x - line.a.x))
                     + ((circle.center.y - line.a.y) * (line.b.y - line.a.y)))
                    / pow(line.Length(), 2);

        float closest_x = line.a.x + (dot * (line.b.x - line.a.x));
        float closest_y = line.a.y + (dot * (line.b.y - line.a.y));
        float theta = atan2f(closest_y - circle.center.y, closest_x - circle.center.x);
        float x_cr = circle.center.x + circle.r * cos(theta);
        float y_cr = circle.center.y + circle.r * sin(theta);

        if(PointLineCollision(Point2D{ static_cast<int32_t>(closest_x), static_cast<int32_t>(closest_y) }, line)) {
            float x1 = circle.center.x - closest_x;
            float y1 = circle.center.y - closest_y;
            float distance = sqrt((x1 * x1) + (y1 * y1));
            if(distance <= circle.r) {
                ret = true;
                ret_point.x = static_cast<int32_t>(closest_x - x_cr);
                ret_point.y = static_cast<int32_t>(closest_y - y_cr);
            }
        }
        return std::make_pair(ret, ret_point + ret_point_1 + ret_point_2);
    }
} // namespace Physics