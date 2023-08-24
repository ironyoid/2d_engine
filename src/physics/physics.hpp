#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include "points.hpp"

struct AABB {
    Point2D position;
    Point2D half_size;
};

struct Object {
    uint32_t id;
    AABB aabb;
};

class StaticObjects
{
    std::vector<Object> objects;

   public:
    StaticObjects();
    uint32_t Add (Object obj);
    void Remove (uint32_t id);
};