#include "robot.hpp"
#include "primitives.hpp"
#include "p8g.hpp"

Robot::Robot(Point2D position, uint32_t radius, float wheel_radius, float mass, float max_torque, float angle) :
    left(0, mass / 2, wheel_radius, max_torque),
    right(0, mass / 2, wheel_radius, max_torque),
    robot(position, radius),
    angle(angle){};

void Robot::Draw(void) {
    p8g::stroke(255, 0, 0, 100);
    p8g::fill(255, 0, 0, 100);
    p8g::ellipseMode(p8g::RADIUS);
    p8g::ellipse(robot.center.x, robot.center.y, robot.r, robot.r);

    p8g::stroke(255, 0, 0, 255);
    p8g::strokeWeight(3);
    float x1 = robot.center.x + robot.r * cos(angle);
    float y1 = robot.center.y + robot.r * sin(angle);
    float x2 = robot.center.x + robot.r * cos(angle + Constants::pi);
    float y2 = robot.center.y + robot.r * sin(angle + Constants::pi);
    p8g::line(x1, y1, x2, y2);
}

void Robot::SetAngle(float _angle) {
    angle = _angle;
}