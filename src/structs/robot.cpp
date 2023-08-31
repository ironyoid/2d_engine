#include "robot.hpp"
#include <cmath>

namespace {
    const float g = 9.8;
    const float rolling_resistance_st = 0.5;
    const float rolling_resistance = 1;
    const float max_rpm = 10;
    const float pi = 3.14;
    constexpr float pi2 = 6.28;
} // namespace

using std::cout;
using std::endl;

Wheel::Wheel(int32_t position, float mass, float radius) : position(position), mass(mass), radius(radius) {
    inertia_moment = 0.5 * mass * pow(radius, 2);
    N = g * mass;
    current_speed = 0.0;
    local_torque = 0.0;
    is_stopping = false;
    count = 0;
    state = eWheelState_Idle;
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
            ret = rolling_resistance_st * current_speed;
            break;
    }
    return ret;
}

float Wheel::GetAngleAcc2(float torque) {
    float acc = (torque - GetCoef() * N) / (inertia_moment + mass * pow(radius, 2));
    return acc;
}

float Wheel::GetAngleAcc(float torque) {
    float acc = 0.0;
    if(abs(torque) >= rolling_resistance * N) {
        acc = (torque - rolling_resistance_st * N) / (inertia_moment + mass * pow(radius, 2));
    }
    cout << "abs = " << fabs(current_speed) << endl;
    if(torque == 0.0 && fabs(current_speed) > 0.0) {
        acc = (-rolling_resistance * N) / (inertia_moment + mass * pow(radius, 2));
    }
    return acc;
}

float Wheel::Run(float torque, float time_delta) {
    float angle_acc = GetAngleAcc2(torque);
    float acc = angle_acc * radius;

    current_angle_speed = current_angle_speed + angle_acc * time_delta;
    current_angle_speed = current_angle_speed < -(max_rpm * pi2) ? -(max_rpm * pi2) : current_angle_speed;
    current_angle_speed = current_angle_speed > (max_rpm * pi2) ? (max_rpm * pi2) : current_angle_speed;
    current_angle_speed = fabs(current_angle_speed) < 0.1 ? 0 : current_angle_speed;

    current_speed = current_angle_speed * radius;
    position = position + current_speed * time_delta + (acc * pow(time_delta, 2)) / 2.0;

    return current_speed;
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
        position = position + current_speed * time_delta + (acc * pow(time_delta, 2)) / 2.0;
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
        cout << "current_speed = " << current_speed << endl;
        cout << "current_angle_speed = " << current_angle_speed << endl;
        cout << "state = " << state << endl;
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

void Wheel::SetTorque(float torque) {
    local_torque = torque;
}