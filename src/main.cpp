#include <iostream>
#include <string>
#include <vector>
#include "p8g.hpp"
#include "parser.hpp"
#include "primitives.hpp"
#include "grid.hpp"
#include <cmath>
#include "physics.hpp"
#include "robot.hpp"

using namespace p8g;
using std::cout;
using std::endl;
using std::vector;

#define WINDOW_WIDTH  (1000U)
#define WINDOW_HEIGHT (1000U)
#define GRID_STEP     (20U)

typedef enum {
    eDrawSate_Idle = 0,
    eDrawSate_Start = 1,
    eDrawState_Proccess = 2,
} eDrawStete_t;

typedef enum {
    eLeft_Button = 0,
    eRight_Button = 1,
} eMouseButtons_t;

typedef enum {
#ifdef WINDOWS_BUILD
    eCtrl_Key = 341,
#else
    eCtrl_Key = 343,
#endif
    eZ_Key = 90,
    eRightArrow_Key = 262,
    eLeftArrow_Key = 263,
    eUpArrow_Key = 265,
    eDownArrow_Key = 264,
    eP_Key = 80,
    eR_Key = 82,
    ePlus_Key = 61,
    eMinus_key = 45,
    eW_key = 87,
    eS_key = 83,
    eE_key = 69,
    eD_key = 68,
} eKeyboardKeys_t;

class Draw
{
   public:
    static Grid *grid;
    static Parser *parser;
    static Wheel *wheel;
    static Robot *robot;
    static vector<Line2D> lines;
    static uint32_t window_width;
    static uint32_t window_height;
    static Point2D position;
    static Point2D mouse;
    static float scale;
    static bool is_right_button_pressed;
    static bool is_ctrl_pressed;
    constexpr static float LINE_THICKNESS = 4.0;
    static const int32_t physics_tick = 5;

    static void Init (uint32_t width,
                      uint32_t height,
                      uint32_t _window_width,
                      uint32_t _window_height,
                      uint32_t step,
                      std::string path) {
        window_width = _window_width;
        window_height = _window_height;
        scale = 1.0;
        parser = new Parser(path);
        wheel = new Wheel(200, 1, 1, 100);
        robot = new Robot(Point2D{ 250, 250 }, 50, 0.1, 2, 100, 0);
        auto [ret_lines, ret_point] = parser->ReadLines(window_width, window_height);
        lines = ret_lines;
        grid = new Grid(step, width, height, _window_width, _window_height);
        grid->GenerateGrid();
    }

    static void UpdateScale (float new_scale) {
        if(scale + new_scale >= 1.0) {
            scale = scale + new_scale;
        } else {
            scale = 1;
        }
        cout << "scale = " << scale << endl;
    }

    static int32_t CheckBoundaries (int32_t val, int32_t delta) {
        int32_t ret = 0;
        if(val - delta < 0) {
            ret = 0;
        } else if((val - delta >= 0) && (val - delta) <= (grid->size_x - grid->window_width)) {
            ret = val - delta;
        } else {
            ret = grid->size_x - grid->window_width;
        }
        return ret;
    }

    static void DrawLines (vector<Line2D> lines, Point2D _position) {
        static bool left_buf = false;
        static bool right_buf = false;
        for(auto n : lines) {
            auto [ret, ret_point] = Physics::LineCircleCollision(n, *robot->GetBody());
            robot->ChangePosition(ret_point);
            auto [left_ray, right_ray] = robot->GetRays();
            auto [ret_ll, ret_point_ll] = Physics::LineLineCollision(*left_ray, n);
            auto [ret_lr, ret_point_lr] = Physics::LineLineCollision(*right_ray, n);
            left_buf = left_buf | ret_ll;
            right_buf = right_buf | ret_lr;
            // cout << "left: " << ret_point_ll.ToString() << " " << ret_point_lr.ToString() << endl;

            // if(true == ret_ll) {
            //     p8g::stroke(0, 0, 255, 255);
            //     p8g::strokeWeight(20);x
            //     p8g::point(ret_point_ll.x, ret_point_ll.y);
            // }
            // if(true == ret_lr) {
            //     p8g::stroke(0, 0, 255, 255);
            //     p8g::strokeWeight(20);
            //     p8g::point(ret_point_lr.x, ret_point_lr.y);
            // }
            p8g::stroke(0, 0, 0, 255);
            p8g::strokeWeight(LINE_THICKNESS);
            p8g::line(n.a.x - _position.x, n.a.y - _position.y, n.b.x - _position.x, n.b.y - _position.y);
        }
        robot->SensorFeedback(left_buf, right_buf);
        right_buf = false;
        left_buf = false;
    }

    static void DrawQuad (Quad2D quad) {
        p8g::strokeWeight(LINE_THICKNESS);
        p8g::line(quad.letf_top.x, quad.letf_top.y, quad.letf_top.x + quad.size.x, quad.letf_top.y);
        p8g::line(quad.letf_top.x + quad.size.x,
                  quad.letf_top.y,
                  quad.letf_top.x + quad.size.x,
                  quad.letf_top.y + quad.size.y);
        p8g::line(quad.letf_top.x + quad.size.x,
                  quad.letf_top.y + quad.size.y,
                  quad.letf_top.x,
                  quad.letf_top.y + quad.size.y);
        p8g::line(quad.letf_top.x, quad.letf_top.y + quad.size.y, quad.letf_top.x, quad.letf_top.y);
    }

    static void DrawCircle (Circle2D circle) {
        p8g::ellipseMode(RADIUS);
        p8g::ellipse(circle.center.x, circle.center.y, circle.r, circle.r);
    }

    static void DrawTask (void) {
        static int last_millis = 0;
        background(250);
        p8g::applyMatrix(1.0, 0, 0, 1, -position.x, -position.y);
        p8g::scale(scale, scale);

        if(last_millis + physics_tick <= p8g::millis()) {
            last_millis = p8g::millis();
            robot->ProcessLeft(physics_tick / 1000.0);
            robot->ProcessRight(physics_tick / 1000.0);
            robot->ProcessSensors(physics_tick / 1000.0);
        }
        robot->Draw();
        DrawLines(lines, position);

        p8g::stroke(0, 0, 0, 120);
        grid->DrawGrid(position);
    };

    static void KeyPressed () {
        static float angle = 0;
        static float left_torque = 0.0;
        static float right_torque = 0.0;
        cout << "key = " << keyCode << endl;
        if(eCtrl_Key == keyCode) {
            is_ctrl_pressed = true;
        }
        if(eW_key == keyCode) {
            left_torque += 10;
        }
        if(eS_key == keyCode) {
            left_torque -= 10;
        }
        if(eE_key == keyCode) {
            right_torque += 10;
        }
        if(eD_key == keyCode) {
            right_torque -= 10;
        }
        robot->SetTorque(left_torque, right_torque);
    };

    static void KeyReleased () {
        switch(keyCode) {
            case eCtrl_Key:
                is_ctrl_pressed = false;
                break;
            case eP_Key:
                for(auto n : lines) {
                    cout << n.ToString() << endl;
                }
                break;
            default:
                break;
        }
    };

    static void MouseReleased () {
        if(eRight_Button == mouseButton) {
            is_right_button_pressed = false;
        }
    };

    static void MouseMoved () {
        if(true == is_right_button_pressed) {
            int32_t delta_x = mouse.x - mouseX;
            int32_t delta_y = mouse.y - mouseY;
            position.x = CheckBoundaries(position.x, -delta_x);
            position.y = CheckBoundaries(position.y, -delta_y);
        }
        mouse.x = mouseX;
        mouse.y = mouseY;
    };
    static void MousePressed () {
        if(eRight_Button == mouseButton) {
            mouse.x = mouseX;
            mouse.y = mouseY;
            is_right_button_pressed = true;
        }
    };
    static void MouseWheel (float delta) {
        if(true == is_ctrl_pressed) {
            UpdateScale(delta / 10);
        }
    };
};

Grid *Draw::grid;
Parser *Draw::parser;
vector<Line2D> Draw::lines;
Point2D Draw::position;
bool Draw::is_ctrl_pressed;
bool Draw::is_right_button_pressed;
Point2D Draw::mouse;
float Draw::scale;
uint32_t Draw::window_width;
uint32_t Draw::window_height;
Wheel *Draw::wheel;
Robot *Draw::robot;

int main (int argc, char *argv[]) {
    if(2 == argc) {
        std::string path(argv[1]);
        Draw::Init(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2, WINDOW_WIDTH, WINDOW_HEIGHT, GRID_STEP, "outlines.txt");
        RunArgs run_args{ WINDOW_WIDTH, WINDOW_HEIGHT, "2d_engine", false };
        p8g::_run(run_args,
                  &Draw::DrawTask,
                  &Draw::KeyPressed,
                  &Draw::KeyReleased,
                  &Draw::MouseMoved,
                  &Draw::MousePressed,
                  &Draw::MouseReleased,
                  &Draw::MouseWheel);
    } else {
        cout << "Wrong number of the parametrs!" << endl;
    }
}