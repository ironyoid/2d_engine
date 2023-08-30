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
    float position;
    float velocity;
    float mass;
    float radius;
    float inertia_moment;
    float resistance_torque;
    float N;
    float current_speed;
    float current_angle_speed;
    float local_torque;
    float GetAcc (float torque);
    float GetAngleAcc (float torque);
    float Run (float torque, float time_delta);
    float Stop (float torque, float time_delta);

   public:
    Wheel(int32_t position, float mass, float radius);
    int32_t GetPosition (void);
    void Proccess (float time_delta);
    void SetTorque (float torque);
};

class Robot
{
    Point2D position;
    Wheel left;
    Wheel right;
    float angle;
};
