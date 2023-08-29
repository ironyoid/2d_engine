#pragma once
#include <iostream>
#include <string>
#include <stdint.h>
#include <cmath>
#include "primitives.hpp"

using std::string;

struct Robot {
    Point2D position;
    int32_t velocity;
    float angle;
};