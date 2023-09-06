#include "robot.hpp"
#include "primitives.hpp"
#include "p8g.hpp"
#include <iostream>
#include <utility>
#include <random>
#include <functional>
#include <algorithm>

using std::cout;
using std::endl;

Robot::Robot(Point2D position, uint32_t radius, float wheel_radius, float mass, float max_torque, float angle) :
    left(0, mass / 2, wheel_radius, max_torque),
    right(0, mass / 2, wheel_radius, max_torque),
    body(position, radius) {
    body_axis.a.x = position.x - radius;
    body_axis.a.y = position.y;
    body_axis.b.x = position.x + radius;
    body_axis.b.y = position.y;
    len = radius * 2;
    angle = Constants::pi;

    ax = body_axis.a.x;
    ay = body_axis.a.y;
    bx = body_axis.b.x;
    by = body_axis.b.y;
    left_sensor_status = false;
    right_sensor_status = false;
    robot_state = eRobotState_Stop;
    //left_ray.b.x = left_ray.a.x + (radius + 30) * cos(0.785398 + Constants::pi);
    //left_ray.b.y = left_ray.a.y + (radius + 30) * sin(0.785398 + Constants::pi);
};

void Robot::Draw(void) {
    p8g::stroke(255, 0, 0, 150);
    p8g::fill(255, 0, 0, 150);
    p8g::ellipseMode(p8g::RADIUS);
    body.center.x = (body_axis.b.x + body_axis.a.x) / 2;
    body.center.y = (body_axis.b.y + body_axis.a.y) / 2;
    p8g::ellipse(body.center.x, body.center.y, body.r, body.r);

    /* p8g::stroke(255, 0, 0, 255);
    p8g::strokeWeight(3);
    p8g::line(body_axis.a.x, body_axis.a.y, body_axis.b.x, body_axis.b.y); */
    left_ray.a.x = body.center.x;
    left_ray.a.y = body.center.y;
    left_ray.b.x = body.center.x + (body.r + 50) * cos(angle + 0.785398 - Constants::pi);
    left_ray.b.y = body.center.y + (body.r + 50) * sin(angle + 0.785398 - Constants::pi);

    right_ray.a.x = body.center.x;
    right_ray.a.y = body.center.y;
    right_ray.b.x = body.center.x + (body.r + 50) * cos(angle + 2.35619 + Constants::pi);
    right_ray.b.y = body.center.y + (body.r + 50) * sin(angle + 2.35619 + Constants::pi);

    p8g::strokeWeight(10);
    p8g::stroke(0, 255, 0, 100);
    p8g::line(left_ray.a.x, left_ray.a.y, left_ray.b.x, left_ray.b.y);
    p8g::line(right_ray.a.x, right_ray.a.y, right_ray.b.x, right_ray.b.y);
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
    angle = angle + theta;
    x = bx + len * cos(angle);
    y = by + len * sin(angle);
    ax = x;
    ay = y;
    body_axis.a.x = x;
    body_axis.a.y = y;
}

void Robot::ProcessRight(float time_delta) {
    float position_delta = right.Process(time_delta);
    float theta = position_delta / len;
    float x = 0.0;
    float y = 0.0;
    angle = angle - theta;
    x = ax + len * cos(angle - Constants::pi);
    y = ay + len * sin(angle - Constants::pi);
    bx = x;
    by = y;
    body_axis.b.x = x;
    body_axis.b.y = y;
}

void Robot::ProcessSensors(float time_delta) {
    std::vector<int>::iterator it = std::find_if(myvector.begin(), myvector.end(), IsOdd);
    static float timer = 0.0;
    static bool last_left_state = false;
    static bool last_right_state = false;
    static auto gen_turn = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
    static auto gen_time = std::bind(std::uniform_real_distribution<>(0.1, 0.4), std::default_random_engine());
    /*     std::cout << "state = " << robot_state << " timer = " << timer << std::endl; */
    switch(robot_state) {
        case eRobotState_Stop:
            break;
        case eRobotState_Idle:
            SetTorque(-20, -20);
            robot_state = eRobotState_Drive;
            break;
        case eRobotState_Drive:
            if(true == left_sensor_status || true == right_sensor_status) {
                robot_state = eRobotState_Revers;
                last_left_state = left_sensor_status;
                last_right_state = right_sensor_status;
            }
            break;
        case eRobotState_Revers:
            timer = 0.6;
            SetTorque(15, 15);
            robot_state = eRobotState_Wait;
            break;
        case eRobotState_Wait:
            if(timer <= 0.0) {
                SetTorque(0, 0);
                timer = 0.2;
                robot_state = eRobotState_Wait2;
            }
            break;
        case eRobotState_Wait2:
            if(timer <= 0.0) {
                if((true == last_left_state) && (true == last_right_state)) {
                    bool g = gen_turn();
                    cout << "gen res = " << g << endl;
                    if(true == g) {
                        robot_state = eRobotState_Left;
                    } else {
                        robot_state = eRobotState_Right;
                    }
                } else if(true == last_left_state) {
                    robot_state = eRobotState_Left;
                } else if(true == last_right_state) {
                    robot_state = eRobotState_Right;
                } else {
                    robot_state = eRobotState_Idle;
                }
            }
            break;

        case eRobotState_Left:
            timer = gen_time();
            cout << "gen time left = " << timer << endl;
            SetTorque(15, -15);
            robot_state = eRobotState_Wait3;
            break;

        case eRobotState_Right:
            timer = gen_time();
            cout << "gen time right = " << timer << endl;
            SetTorque(-15, 15);
            robot_state = eRobotState_Wait3;
            break;

        case eRobotState_Wait3:
            if(timer <= 0.0) {
                if(true == left_sensor_status) {
                    robot_state = eRobotState_Left;
                }
                if(true == right_sensor_status) {
                    robot_state = eRobotState_Right;
                }
                if(false == right_sensor_status && false == left_sensor_status) {
                    robot_state = eRobotState_Idle;
                }
            }
            break;
    }

    if(timer >= 0) {
        timer = timer - time_delta;
    }
}

void Robot::ChangePosition(Point2D delta_pos) {
    body_axis.a.x += delta_pos.x;
    body_axis.a.y += delta_pos.y;
    body_axis.b.x += delta_pos.x;
    body_axis.b.y += delta_pos.y;

    ax += delta_pos.x;
    ay += delta_pos.y;
    bx += delta_pos.x;
    by += delta_pos.y;
}

Circle2D *Robot::GetBody(void) {
    return &body;
}

std::pair<const Line2D *, const Line2D *> Robot::GetRays(void) {
    return std::make_pair(&left_ray, &right_ray);
}

void Robot::SensorFeedback(bool left_ray, bool right_ray) {
    right_sensor_status = right_ray;
    left_sensor_status = left_ray;
    /* std::cout << "right sensor = " << right_sensor_status << " left sensor = " << left_sensor_status << std::endl; */
}

void Robot::Run(void) {
    robot_state = eRobotState_Idle;
}