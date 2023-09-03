#include "wheel.hpp"
#include <cmath>
#include <iostream>

namespace {
    const float rolling_resistance_st = 0.005;
    const float rolling_resistance = 0.01;
    const float max_rpm = 10;

} // namespace

using std::cout;
using std::endl;

Wheel::Wheel(int32_t position, float mass, float radius, float max_torque) :
    max_torque(max_torque),
    position(position),
    mass(mass),
    radius(radius) {
    inertia_moment = 0.5 * mass * powf(radius, 2);
    N = Constants::g * mass;
    current_speed = 0.0;
    torque = 0.0;
    state = eWheelState_Idle;
    angle_position = 0.0;
    encoder = 0;
}

float Wheel::GetCoef(void) {
    float ret = 0.0;
    switch(state) {
        case eWheelState_Idle:
            ret = fabs(torque / N);
            if(ret > rolling_resistance) {
                state = eWheelState_Motion;
                ret = rolling_resistance_st;
            }
            break;
        case eWheelState_Motion:
            ret = 100 * rolling_resistance_st * current_speed;
            break;
        case eWheelState_Stop:
            ret = 100 * rolling_resistance_st * current_speed;
            break;
    }
    return ret;
}

float Wheel::GetAngleAcc(float torque) {
    float acc = (torque - GetCoef() * N) / (inertia_moment + mass * powf(radius, 2));
    return acc;
}

void Wheel::UpdateEncoder(void) {
    float tmp = angle_position / Constants::pi2;
    encoder = static_cast<uint32_t>(tmp);
}

float Wheel::Run(float torque, float time_delta) {
    float angle_acc = GetAngleAcc(torque);
    float acc = angle_acc * radius;
    float angle_position_delta = 0.0;
    float position_delta = 0.0;
    current_angle_speed = current_angle_speed + angle_acc * time_delta;
    // current_angle_speed = current_angle_speed < -(max_rpm * pi2) ? -(max_rpm * pi2) : current_angle_speed;
    // current_angle_speed = current_angle_speed > (max_rpm * pi2) ? (max_rpm * pi2) : current_angle_speed;
    current_angle_speed = fabs(current_angle_speed) < 0.001 ? 0 : current_angle_speed;
    current_speed = current_angle_speed * radius;
    position_delta = (current_speed * time_delta + (acc * powf(time_delta, 2)) / 2.0) * 100;
    position += position_delta;
    angle_position_delta = current_angle_speed * time_delta + (angle_acc * powf(time_delta, 2)) / 2.0;
    angle_position += angle_position_delta;
    UpdateEncoder();
    return position_delta;
}

float Wheel::Process(float time_delta) {
    static uint8_t count = 0;
    float position_delta = 0.0;
    position_delta = Run(torque, time_delta);

    if(count == 10) {
        cout << "torque = " << torque << endl;
        cout << "time delta = " << time_delta << endl;
        cout << "position = " << position << endl;
        cout << "angle_position = " << angle_position << endl;
        cout << "current_speed = " << current_speed << endl;
        cout << "current_angle_speed = " << current_angle_speed << endl;
        cout << "state = " << state << endl;
        cout << "encoder = " << encoder << endl;
        cout << endl;
        count = 0;
    }

    if(fabs(current_speed) == 0.0) {
        state = eWheelState_Idle;
    } else if(torque != 0) {
    } else {
        state = eWheelState_Stop;
    }
    count += 1;
    return position_delta;
}

int32_t Wheel::GetPosition(void) {
    return static_cast<int32_t>(position);
}

float Wheel::GetAnglePosition(void) {
    return angle_position;
}

float Wheel::GetRadius(void) {
    return radius;
}

void Wheel::SetTorque(float _torque) {
    torque = _torque > max_torque ? max_torque : _torque;
    torque = _torque < -max_torque ? -max_torque : _torque;
}

uint32_t Wheel::GetEncoder(void) {
    return encoder;
}