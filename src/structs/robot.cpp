#include "robot.hpp"
#include <synchapi.h>
/* #include <corecrt_math.h> */
#include <cmath>
#include "windows.h"

namespace {
    const float g = 9.8;
    const float rolling_resistance_st = 0.005;
    const float rolling_resistance = 0.01;
    const float max_rpm = 10;
    const float pi = 3.14;
    constexpr float pi2 = 6.28;
} // namespace

using std::cout;
using std::endl;

Wheel::Wheel(int32_t position, float mass, float radius) : position(position), mass(mass), radius(radius) {
    inertia_moment = 0.5 * mass * powf(radius, 2);
    N = g * mass;
    current_speed = 0.0;
    local_torque = 0.0;
    is_stopping = false;
    count = 0;
    state = eWheelState_Idle;
    angle_position = 0.0;
}

float Wheel::GetCoef(void) {
    float ret = 0.0;
    switch(state) {
        case eWheelState_Idle:
            ret = fabs(local_torque / N);
            if(ret > rolling_resistance) {
                state = eWheelState_Motion;
                ret = rolling_resistance_st;
            }
            break;
        case eWheelState_Motion:
            ret = rolling_resistance_st;
            break;
        case eWheelState_Stop:
            ret = 100 * rolling_resistance_st * current_speed;
            break;
    }
    return ret;
}

float Wheel::GetAngleAcc2(float torque) {
    float acc = (torque - GetCoef() * N) / (inertia_moment + mass * powf(radius, 2));
    return acc;
}

float Wheel::GetAngleAcc(float torque) {
    float acc = 0.0;
    if(abs(torque) >= rolling_resistance * N) {
        acc = (torque - rolling_resistance_st * N) / (inertia_moment + mass * powf(radius, 2));
    }
    cout << "abs = " << fabs(current_speed) << endl;
    if(torque == 0.0 && fabs(current_speed) > 0.0) {
        acc = (-rolling_resistance * N) / (inertia_moment + mass * powf(radius, 2));
    }
    return acc;
}

float Wheel::Run(float torque, float time_delta) {
    float angle_acc = GetAngleAcc2(torque);
    float acc = angle_acc * radius;
    float angle_position_delta = 0.0;

    current_angle_speed = current_angle_speed + angle_acc * time_delta;

    current_angle_speed = current_angle_speed < -(max_rpm * pi2) ? -(max_rpm * pi2) : current_angle_speed;
    current_angle_speed = current_angle_speed > (max_rpm * pi2) ? (max_rpm * pi2) : current_angle_speed;
    current_angle_speed = fabs(current_angle_speed) < 0.001 ? 0 : current_angle_speed;

    current_speed = current_angle_speed * radius;
    position = position + (current_speed * time_delta + (acc * powf(time_delta, 2)) / 2.0) * 100;

    angle_position_delta = current_angle_speed * time_delta + (angle_acc * powf(time_delta, 2)) / 2.0;
    angle_position += angle_position_delta;

    return current_speed;
}

float Wheel::CalculatePosition(float angle) {
    return angle * radius * pi2 * 100;
}

float Wheel::Stop(float torque, float time_delta) {
    float angle_acc = GetAngleAcc(torque);
    float acc = angle_acc * radius;
    current_angle_speed = current_angle_speed + angle_acc * time_delta;
    current_speed = current_speed + acc * time_delta;
    if(current_angle_speed <= 0) {
        current_speed = 0.0;
        current_angle_speed = 0.0;
    } else {
        position = position + current_speed * time_delta + (acc * powf(time_delta, 2)) / 2.0;
    }
    cout << "position = " << position << endl;
    return current_speed;
}

void Wheel::Proccess(float time_delta) {
    static uint8_t count = 0;

    Run(local_torque, time_delta);

    if(count == 10) {
        cout << "torque = " << local_torque << endl;
        cout << "time delta = " << time_delta << endl;
        cout << "position = " << position << endl;
        cout << "angle_position = " << angle_position << endl;
        cout << "current_speed = " << current_speed << endl;
        cout << "current_angle_speed = " << current_angle_speed << endl;
        cout << "state = " << state << endl;
        cout << endl;
        count = 0;
    }

    if(fabs(current_speed) == 0.0) {
        state = eWheelState_Idle;
    } else if(local_torque != 0) {
    } else {
        state = eWheelState_Stop;
    }
    count += 1;
}

int32_t Wheel::GetPosition(void) {
    return static_cast<int32_t>(position);
}

float Wheel::GetAnglePosition(void) {
    return angle_position;
}

void Wheel::SetTorque(float torque) {
    local_torque = torque;
}