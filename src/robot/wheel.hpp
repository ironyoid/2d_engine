#pragma once
#include <_types/_uint32_t.h>
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
    float mass;
    float radius;
    float inertia_moment;
    float N;
    float current_speed;
    float current_angle_speed;
    float torque;
    float max_torque;
    uint32_t encoder;
    typedef enum {
        eWheelState_Idle = 0,
        eWheelState_Motion = 1,
        eWheelState_Stop = 2,
    } eWheelState_t;
    eWheelState_t state;

    float GetAngleAcc (float torque);
    float GetCoef (void);
    void Run (float torque, float time_delta);
    void UpdateEncoder (void);

   public:
    Wheel(int32_t position, float mass, float radius, float max_torque);
    int32_t GetPosition (void);
    float GetAnglePosition (void);
    float GetRadius (void);
    void Proccess (float time_delta);
    void SetTorque (float torque);
    float CalculatePosition (float angle);
    uint32_t GetEncoder (void);
};
