#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include "primitives.hpp"

struct AABB {
    Point2D position;
    Point2D half_size;
};

template<typename T> struct Object {
    T data;
    uint32_t id;
};

template<typename T> class Objects
{
   public:
    using VectorType = std::vector<Object<T>>;

   protected:
    VectorType objects;
    uint32_t counter;

   public:
    Objects(VectorType objects) :
        objects{ objects },
        counter{ 0 } {

        };

    typename VectorType::iterator begin () {
        return objects.begin();
    }
    typename VectorType::iterator end () {
        return objects.end();
    }

    uint32_t Add (T element) {
        uint32_t tmp_cnt = counter;
        Object<T> tmp_obj{ element, counter };
        objects.push_back(tmp_obj);
        ++counter;
        return tmp_cnt;
    }

    void Remove (uint32_t id) {
        objects.erase(objects.begin() + id);
    }
};

class DynamicObjects : public Objects<Robot2D>
{
    DynamicObjects(VectorType _objects) : Objects<Robot2D>{ _objects } {
    }
};

std::vector<Point2D> MinkovskiDifference (Line2D line, Quad2D quad);
Quad2D MinkovskiDifference2 (Quad2D quad1, Quad2D quad2);
std::pair<bool, Point2D> LineCircleCollision (Line2D line, Circle2D circle);
bool PointLineCollision (Point2D point, Line2D line);
std::pair<bool, Point2D> PointCircleCollision (Point2D point, Circle2D circle);