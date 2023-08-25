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
    AABB aabb;
    string ToString (void) {
        string tmp = aabb.position.ToString() + " " + aabb.half_size.ToString();
        return tmp;
    }
};

class StaticObjects
{
   private:
    struct StoredObject {
        Object object;
        uint32_t id;
    };
    using ObjectType = std::vector<StoredObject>;
    ObjectType objects;
    uint32_t counter;

    Object LineToObject (Line2D line);

   public:
    StaticObjects() = default;
    ObjectType::iterator begin ();
    ObjectType::iterator end ();
    uint32_t Add (Object obj);
    void Remove (uint32_t id);
};