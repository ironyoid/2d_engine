#pragma once
#include "primitives.hpp"
#include "wheel.hpp"

class Robot
{
   private:
    Wheel left;
    Wheel right;
    Circle2D body;
    Line2D line;
    float angle;
    float len;
    float angle_left;
    float angle_right;

    float ax, ay, bx, by;
    float last_ax, last_ay, last_bx, last_by;

   public:
    Robot(Point2D position, uint32_t radius, float wheel_radius, float mass, float max_torque, float angle);
    void SetAngle (float _angle);
    void Draw (void);
    void Draw2 (void);
    void ProcessLeft (float time_delta);
    void ProcessRight (float time_delta);
    void SetTorque (float _left, float _right);
    Circle2D *GetBody (void);
};