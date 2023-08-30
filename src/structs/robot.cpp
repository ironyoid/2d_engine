#include "robot.hpp"
#include <cmath>

namespace {
    const float g = 9.8;
    const float rolling_resistance_st = 0.0005;
    const float rolling_resistance = 0.001;
} // namespace

Wheel::Wheel(int32_t position, float mass, float radius) : position(position), mass(mass), radius(radius) {
    inertia_moment = 0.5 * mass * pow(radius, 2);
    N = g * mass;
    resistance_torque = rolling_resistance_st * N;
    current_speed = 0.0;
}

float Wheel::GetAcc(float torque) {
    float acc = ((torque - N * inertia_moment) * radius) / (inertia_moment + mass * pow(radius, 2));
    return acc;
}

float Wheel::GetAngleAcc(float torque) {
    float acc = (resistance_torque + torque - rolling_resistance_st * N) / (inertia_moment + mass * pow(radius, 2));
    return acc;
}

float Wheel::Run(float torque, float time_delta) {
    float angle_acc = GetAngleAcc(torque);
    float acc = angle_acc * radius;
    current_angle_speed = current_angle_speed + angle_acc * time_delta;
    current_speed = current_speed + acc * time_delta;
    position = static_cast<int32_t>(position + acc * pow(time_delta, 2));
    return current_speed;
}

float Wheel::Stop(float torque, float time_delta) {
    float speed = Run(torque, time_delta);
    if(speed <= 0.0) {
        current_angle_speed = 0.0;
        current_speed = 0.0;
    }
    return current_speed;
}

int32_t Wheel::GetPosition(void) {
    return position;
}