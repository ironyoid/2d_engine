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
    float angle_position;
    float velocity;
    float mass;
    float radius;
    float inertia_moment;
    float resistance_torque;
    float N;
    float current_speed;
    float current_angle_speed;
    float local_torque;
    bool is_stopping;
    uint32_t count;

    typedef enum {
        eWheelState_Idle = 0,
        eWheelState_Motion = 1,
        eWheelState_Stop = 2,
    } eWheelState_t;
    eWheelState_t state;

    float GetAngleAcc (float torque);
    float GetAngleAcc2 (float torque);
    float GetCoef (void);
    float Run (float torque, float time_delta);
    float Stop (float torque, float time_delta);

   public:
    Wheel(int32_t position, float mass, float radius);
    int32_t GetPosition (void);
    float GetAnglePosition (void);
    void Proccess (float time_delta);
    void SetTorque (float torque);
    float CalculatePosition (float angle);
};

class Robot
{
    Point2D position;
    Wheel left;
    Wheel right;
    float angle;
};
