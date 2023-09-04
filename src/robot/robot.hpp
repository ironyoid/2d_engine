#pragma once
#include "primitives.hpp"
#include "wheel.hpp"
#include <utility>

class Robot
{
   private:
    typedef enum {
        eRobotState_Idle,
        eRobotState_Drive,
        eRobotState_Left,
        eRobotState_Right,
        eRobotState_Revers,
        eRobotState_Wait,
        eRobotState_Wait2,
        eRobotState_Wait3,
    } eRobotState_t;

    Wheel left;
    Wheel right;
    float ax, ay, bx, by;
    float angle;
    float len;
    float last_ax, last_ay, last_bx, last_by;
    Circle2D body;
    Line2D body_axis;
    Line2D left_ray;
    Line2D right_ray;
    bool left_sensor_status;
    bool right_sensor_status;
    eRobotState_t robot_state;

   public:
    Robot(Point2D position, uint32_t radius, float wheel_radius, float mass, float max_torque, float angle);
    void Draw (void);
    void ProcessLeft (float time_delta);
    void ProcessRight (float time_delta);
    void ProcessSensors (float time_delta);
    void SetTorque (float _left, float _right);
    void ChangePosition (Point2D delta_pos);
    Circle2D *GetBody (void);
    std::pair<const Line2D *, const Line2D *> GetRays (void);
    void SensorFeedback (bool left_ray, bool right_ray);
};