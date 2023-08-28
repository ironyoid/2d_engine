#include "physics.hpp"
#include "p8g.hpp"
#include "points.hpp"
#include <utility>
#include <vector>
#include <cmath>

StaticObjects::ObjectType::iterator StaticObjects::begin() {
    return objects.begin();
}

StaticObjects::ObjectType::iterator StaticObjects::end() {
    return objects.end();
}

uint32_t StaticObjects::Add(Object obj) {
    uint32_t tmp_cnt = counter;
    StoredObject tmp{ obj, counter };
    objects.push_back(tmp);
    counter += 1;
    return tmp_cnt;
}

void StaticObjects::Remove(uint32_t id) {
    objects.erase(objects.begin() + id);
}

Object StaticObjects::LineToObject(Line2D line) {
    Object ret;
    return ret;
}

std::vector<Point2D> MinkovskiDifference (Line2D line, Quad2D quad) {
    std::vector<Point2D> ret;
    Point2D a1 = line.a - quad.letf_top;
    Point2D a2 = line.a - Point2D{ quad.letf_top.x + quad.size.x, quad.letf_top.y };
    Point2D a3 = line.a - Point2D{ quad.letf_top.x + quad.size.x, quad.letf_top.y + quad.size.y };
    Point2D a4 = line.a - Point2D{ quad.letf_top.x, quad.letf_top.y + quad.size.y };

    Point2D b1 = line.b - quad.letf_top;
    Point2D b2 = line.b - Point2D{ quad.letf_top.x + quad.size.x, quad.letf_top.y };
    Point2D b3 = line.b - Point2D{ quad.letf_top.x + quad.size.x, quad.letf_top.y + quad.size.y };
    Point2D b4 = line.b - Point2D{ quad.letf_top.x, quad.letf_top.y + quad.size.y };

    ret.push_back(a1);
    ret.push_back(a2);
    ret.push_back(a3);
    ret.push_back(a4);

    ret.push_back(b1);
    ret.push_back(b2);
    ret.push_back(b3);
    ret.push_back(b4);

    return ret;
}

Quad2D MinkovskiDifference2 (Quad2D quad1, Quad2D quad2) {
    Point2D a;
    Point2D b;
    a.x = quad1.letf_top.x - quad2.letf_top.x - quad2.size.x;
    a.y = quad2.letf_top.y - quad1.letf_top.y - quad1.size.y;

    b.x = quad1.size.x + quad2.size.x;
    b.y = quad1.size.y + quad2.size.y;

    return Quad2D{ a, b };
}

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
    if(true == ret_1) {
        ret = true;
        ret_point = ret_point_1;
    } else if(true == ret_2) {
        ret = true;
        ret_point = ret_point_2;
    } else {
        float dot = (((circle.center.x - line.a.x) * (line.b.x - line.a.x))
                     + ((circle.center.y - line.a.y) * (line.b.y - line.a.y)))
                    / pow(line.Length(), 2);

        float closest_x = line.a.x + (dot * (line.b.x - line.a.x));
        float closest_y = line.a.y + (dot * (line.b.y - line.a.y));
        // p8g::strokeWeight(4);
        // p8g::line(closest_x, closest_y, circle.center.x, circle.center.y);
        float theta = atan2f(closest_y - circle.center.y, closest_x - circle.center.x);
        float x_cr = circle.center.x + circle.r * cos(theta);
        float y_cr = circle.center.y + circle.r * sin(theta);

        if(PointLineCollision(Point2D{ static_cast<int32_t>(closest_x), static_cast<int32_t>(closest_y) }, line)) {
            float x1 = circle.center.x - closest_x;
            float y1 = circle.center.y - closest_y;
            float distance = sqrt((x1 * x1) + (y1 * y1));
            // if(distance <= circle.r) {
            ret = true;
            ret_point.x = static_cast<int32_t>(closest_x - x_cr);
            ret_point.y = static_cast<int32_t>(closest_y - y_cr);
            p8g::strokeWeight(6);
            p8g::stroke(0, 255, 0, 255);
            p8g::point(closest_x - x_cr, closest_y - y_cr);
            std::cout << ret_point.x << ", " << ret_point.y << " ";
            // }
        }
    }
    std::cout << ret << std::endl;
    return std::make_pair(ret, ret_point);
}