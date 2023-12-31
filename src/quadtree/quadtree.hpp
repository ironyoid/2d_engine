#pragma once
#include <iostream>
#include <string>
#include <stdint.h>

struct Point2D {
    int32_t x;
    int32_t y;
    bool operator==(const Point2D &other) const;
    Point2D &operator=(const Point2D &other);
    Point2D &operator/=(const Point2D &other);
    Point2D &operator/=(const float other);
    Point2D operator/(const float other);
    Point2D operator-(const Point2D &other);
    Point2D operator+(const Point2D &other);
    Point2D operator*(const float other);
    void Print (void);
};

struct Line2D {
    Point2D a;
    Point2D b;
    float thickness;
    bool operator==(const Line2D &other) const;
};

class QuadTree
{
   public:
    Point2D left_top;
    Point2D size;
    Point2D right_bottom;
    QuadTree *top_left_tree;
    QuadTree *top_right_tree;
    QuadTree *bottom_left_tree;
    QuadTree *bottom_right_tree;

    QuadTree();
    QuadTree(Point2D left_top, Point2D right_bottom);
    uint32_t GridSize (void);
    void Split (void);
};

void CreateQuadTree (QuadTree *root, uint32_t grid_size);