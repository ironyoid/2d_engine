#pragma once
#include <iostream>
#include <string>
#include <stdint.h>
#include <cmath>
#include "p8g.hpp"

using std::string;

namespace Constants {
    constexpr float pi = 3.141592;
    constexpr float pi2 = 2 * pi;
    const float g = 9.8;
} // namespace Constants
struct Point2D {
    int32_t x;
    int32_t y;
    bool operator==(const Point2D &other) const {
        return (x == other.x && y == other.y);
    }

    Point2D &operator=(const Point2D &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    Point2D operator/(const float other) {
        Point2D tmp;
        tmp.x = x / other;
        tmp.y = y / other;
        return tmp;
    }

    Point2D operator-(const Point2D &other) {
        Point2D tmp;
        tmp.x = x - other.x;
        tmp.y = y - other.y;
        return tmp;
    }

    Point2D operator+(const Point2D &other) {
        Point2D tmp;
        tmp.x = x + other.x;
        tmp.y = y + other.y;
        return tmp;
    }

    Point2D &operator/=(const float other) {
        x /= other;
        y /= other;
        return *this;
    }

    Point2D operator*(const float other) {
        Point2D tmp;
        tmp.x = x * other;
        tmp.y = y * other;
        return tmp;
    }
    string ToString (void) {
        string tmp = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        return tmp;
    };

    string ToStringNormalized (Point2D position) {
        float ax = static_cast<float>(x) / position.x;
        float ay = static_cast<float>(y) / position.y;
        string tmp = std::to_string(ax) + "," + std::to_string(ay);
        return tmp;
    };
};

struct Quad2D {
    Point2D letf_top;
    Point2D size;
};

struct Circle2D {
    Point2D center;
    uint32_t r;
    Circle2D() = default;
    Circle2D(Point2D center, uint32_t radius) : center(center), r(radius){};
};

struct Line2D {
    Point2D a;
    Point2D b;
    float thickness;
    bool operator==(const Line2D &other) const {
        return (a == other.a && b == other.b);
    }
    string ToString (void) {
        string tmp = a.ToString() + ";" + b.ToString();
        return tmp;
    };
    string ToStringNormalized (Point2D position) {
        float ax = static_cast<float>(a.x) / position.x;
        float bx = static_cast<float>(b.x) / position.x;
        float ay = static_cast<float>(a.y) / position.y;
        float by = static_cast<float>(b.y) / position.y;
        string tmp = std::to_string(ax) + "," + std::to_string(ay) + ";";
        tmp += std::to_string(bx) + "," + std::to_string(by);
        return tmp;
    };
    float Length (void) {
        float ret = sqrt((powf((b.x - a.x), 2) + powf((b.y - a.y), 2)));
        return ret;
    }
};