#pragma once
#include <iostream>
#include <string>
#include <stdint.h>
#include <cmath>
#include "primitives.hpp"

using std::string;

class Wheel
{
   private:
    int32_t position;
    float velocity;
    float mass;
    float radius;
    float inertia_moment;
    float resistance_torque;
    float N;
    float current_speed;
    float current_angle_speed;

    float GetAcc (float torque);
    float GetAngleAcc (float torque);

   public:
    Wheel(int32_t position, float mass, float radius);
    float Run (float torque, float time_delta);
    float Stop (float torque, float time_delta);
    int32_t GetPosition (void);
};

class Robot
{
    Point2D position;
    Wheel left;
    Wheel right;
    float angle;
};
