#pragma once
#include "primitives.hpp"
#include "wheel.hpp"

class Robot
{
   private:
    Wheel left;
    Wheel right;
    Circle2D robot;
    float angle;

   public:
    Robot(Point2D position, uint32_t radius, float wheel_radius, float mass, float max_torque, float angle);
    void SetAngle (float _angle);
    void Draw (void);
};