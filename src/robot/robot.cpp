#include "robot.hpp"
#include "primitives.hpp"
#include "p8g.hpp"
#include <iostream>

using std::cout;
using std::endl;

Robot::Robot(Point2D position, uint32_t radius, float wheel_radius, float mass, float max_torque, float angle) :
    left(0, mass / 2, wheel_radius, max_torque),
    right(0, mass / 2, wheel_radius, max_torque),
    body(position, radius),
    angle(angle) {
    line.a.x = 250;
    line.a.y = 250;
    line.b.x = 350;
    line.b.y = 250;
    len = 100;
    angle_left = 0;
    angle_right = Constants::pi;

    ax = line.a.x;
    ay = line.a.y;
    bx = line.b.x;
    by = line.b.y;

    last_ax = line.a.x;
    last_ay = line.a.y;
    last_bx = line.b.x;
    last_by = line.b.y;
};

void Robot::Draw(void) {
    p8g::stroke(255, 0, 0, 100);
    p8g::fill(255, 0, 0, 100);
    p8g::ellipseMode(p8g::RADIUS);
    body.center.x = (line.a.x + line.b.x) / 2;
    body.center.y = (line.a.y + line.b.y) / 2;
    p8g::ellipse(body.center.x, body.center.y, len / 2, len / 2);

    p8g::stroke(255, 0, 0, 255);
    p8g::strokeWeight(3);
    p8g::line(line.a.x, line.a.y, line.b.x, line.b.y);
}

void Robot::Draw2() {
    p8g::stroke(255, 0, 0, 255);
    p8g::strokeWeight(3);
    p8g::line(line.a.x, line.a.y, line.b.x, line.b.y);
}

void Robot::SetAngle(float _angle) {
    angle = _angle;
}

void Robot::SetTorque(float _left, float _right) {
    left.SetTorque(_left);
    right.SetTorque(_right);
}

void Robot::ProcessLeft(float time_delta) {
    float position_delta = left.Process(time_delta);
    float theta = position_delta / len;

    float x = 0.0;
    float y = 0.0;
    angle_right = angle_right + theta;

    x = bx + len * cos(angle_right);
    y = by + len * sin(angle_right);
    ax = x;
    ay = y;
    line.a.x = x;
    line.a.y = y;

    //cout << "right = " << line.ToString() << endl;
}

void Robot::ProcessRight(float time_delta) {
    float position_delta = right.Process(time_delta);
    float theta = position_delta / len;

    float x = 0.0;
    float y = 0.0;
    angle_right = angle_right - theta;

    x = ax + len * cos(angle_right - Constants::pi);
    y = ay + len * sin(angle_right - Constants::pi);
    bx = x;
    by = y;
    line.b.x = x;
    line.b.y = y;

    //cout << "left = " << line.ToString() << endl;
}

Circle2D *Robot::GetBody(void) {
    return &body;
}