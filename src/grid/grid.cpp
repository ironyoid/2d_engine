#include "grid.hpp"
#include <stdint.h>
#include <iostream>
#include <utility>
#include <vector>
#include "p8g.hpp"
#include "points.hpp"
#include <cmath>
using namespace p8g;
using std::cout;
using std::endl;

Grid::Grid(int32_t step, int32_t size_x, int32_t size_y, uint32_t window_width, uint32_t window_height) :
    step(step),
    size_x(size_x),
    size_y(size_y),
    window_width(window_width),
    window_height(window_height) {
    last_position.x = 0;
    last_position.y = 0;
};

void Grid::GenerateGrid(void) {
    uint32_t thick_counter = 0;
    float tmp_thickness = 0;

    for(int32_t x = 0; x < size_x; x += step) {
        if(10 == thick_counter) {
            thick_counter = 0;
            tmp_thickness = 2.0;
        } else {
            tmp_thickness = 1.0;
        }

        grid_x.push_back(Line2D{ x, 0, x, size_y, tmp_thickness });
        thick_counter++;
    }
    thick_counter = 0;
    for(int32_t y = 0; y < size_y; y += step) {
        if(10 == thick_counter) {
            thick_counter = 0;
            tmp_thickness = 2.0;
        } else {
            tmp_thickness = 1.0;
        }
        grid_y.push_back(Line2D{ 0, y, size_x, y, tmp_thickness });
        thick_counter++;
    }
}

void Grid::DrawGrid(Point2D position) {
    for(auto n : grid_x) {
        if(n.a.x > (window_width + position.x)) {
            break;
        }
        p8g::strokeWeight(n.thickness);
        p8g::line(n.a.x, n.a.y, n.b.x, n.b.y);
    }
    for(auto n : grid_y) {
        if(n.a.y > (window_height + position.y)) {
            break;
        }
        p8g::strokeWeight(n.thickness);
        p8g::line(n.a.x, n.a.y, n.b.x, n.b.y);
    }
}

Point2DDistance Grid::Distance(Point2D point, int32_t mouse_x, int32_t mouse_y, Point2D org) {
    float x = point.x - mouse_x;
    float y = point.y - mouse_y;
    Point2DDistance ret{ org, (int32_t) (abs(x) + abs(y)) };
    return ret;
};

std::optional<Point2D> Grid::FindPoint(int32_t mouse_x,
                                       int32_t mouse_y,
                                       uint32_t threshold,
                                       Point2D position,
                                       float scale) {
    int32_t step_tmp = static_cast<int32_t>(step * scale);
    Point2DDistance ret{ 0, 0, step_tmp };

    int32_t mouse_x_tmp = ((mouse_x + position.x) / step_tmp);
    int32_t mouse_y_tmp = ((mouse_y + position.y) / step_tmp);

    Point2D left_top{ mouse_x_tmp * step_tmp, mouse_y_tmp * step_tmp };
    Point2D right_top{ left_top.x + step_tmp, left_top.y };
    Point2D left_bottom{ left_top.x, left_top.y + step_tmp };
    Point2D right_bottom{ right_top.x, left_top.y + step_tmp };

    Point2D left_top_org{ ((mouse_x_tmp) *step), ((mouse_y_tmp) *step) };
    Point2D right_top_org{ left_top_org.x + step, left_top_org.y };
    Point2D left_bottom_org{ left_top_org.x, left_top_org.y + step };
    Point2D right_bottom_org{ right_top_org.x, left_top_org.y + step };

    std::vector<Point2DDistance> vec;
    vec.push_back(Distance(left_top, mouse_x + (position.x), mouse_y + (position.y), left_top_org));
    vec.push_back(Distance(right_top, mouse_x + (position.x), mouse_y + (position.y), right_top_org));
    vec.push_back(Distance(left_bottom, mouse_x + (position.x), mouse_y + (position.y), left_bottom_org));
    vec.push_back(Distance(right_bottom, mouse_x + (position.x), mouse_y + (position.y), right_bottom_org));

    for(auto n : vec) {
        if(ret.distance > n.distance) {
            ret.a = n.a;
            ret.distance = n.distance;
        }
    }

    if(ret.distance < threshold) {
        return Point2D{ ret.a.x, ret.a.y };
    } else {
        return {};
    }
};
