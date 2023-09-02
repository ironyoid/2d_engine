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
} eKeyboardKeys_t;

class Draw
{
   public:
    static Grid *grid;
    static Parser *parser;
    static Wheel *wheel;
    static Robot *robot;
    static uint32_t window_width;
    static uint32_t window_height;
    static vector<Line2D> lines;
    static Circle2D robot_circle;
    static Point2D position;
    static Point2D mouse;
    static float scale;
    static bool is_right_button_pressed;
    static bool is_ctrl_pressed;
    constexpr static float LINE_THICKNESS = 4.0;

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
        robot = new Robot(Point2D{ 250, 250 }, 20, 0.1, 2, 100, 0);
        auto [ret_lines, ret_point] = parser->ReadLines(window_width, window_height);
        /* lines.push_back(Line2D{ 250, 250, 1000, 250 }); */
        lines = ret_lines;
        robot_circle.center = ret_point;
        robot_circle.r = 20;
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
        for(auto n : lines) {
            auto [ret, ret_point] = LineCircleCollision(n, robot_circle);
            robot_circle.center.x += ret_point.x;
            robot_circle.center.y += ret_point.y;

            p8g::strokeWeight(LINE_THICKNESS);
            p8g::line(n.a.x - _position.x, n.a.y - _position.y, n.b.x - _position.x, n.b.y - _position.y);
        }
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
    static void ProccessMovement (void) {
        int dx = mouse.x - robot_circle.center.x;
        int dy = mouse.y - robot_circle.center.y;
        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

        float inc_x = dx / (float) steps;
        float inc_y = dy / (float) steps;
        if((mouse.x != robot_circle.center.x) || (mouse.y != robot_circle.center.y)) {
            Line2D len_line{ robot_circle.center.x, robot_circle.center.y, mouse.x, mouse.y };
            int32_t stx = inc_x * (len_line.Length() / 10);
            int32_t sty = inc_y * (len_line.Length() / 10);
            if(stx >= 10) {
                stx = 10;
            } else if(stx <= -10) {
                stx = -10;
            }
            if(sty >= 10) {
                sty = 10;
            } else if(sty <= -10) {
                sty = -10;
            }
            robot_circle.center.x += stx;
            robot_circle.center.y += sty;
        }
    }

    static void DrawTask (void) {
        static int last_millis = 0;
        background(250);
        robot->Draw();
        // Circle2D circle{ Point2D{ 0, 200 }, static_cast<uint32_t>(wheel->GetRadius() * 100) };
        // if(last_millis + 5 <= p8g::millis()) {
        //     last_millis = p8g::millis();
        //     wheel->Proccess(0.005);
        // }
        // circle.center.x = wheel->GetPosition();
        // p8g::stroke(255, 0, 0, 100);
        // p8g::fill(255, 0, 0, 100);
        // DrawCircle(circle);

        // p8g::stroke(255, 0, 0, 255);
        // p8g::strokeWeight(2);
        // float theta = wheel->GetAnglePosition();
        // float x_cr = circle.center.x + circle.r * cos(theta);
        // float y_cr = circle.center.y + circle.r * sin(theta);
        // p8g::line(circle.center.x, circle.center.y, x_cr, y_cr);

        /* ProccessMovement(); */

        p8g::applyMatrix(1.0, 0, 0, 1, -position.x, -position.y);
        p8g::scale(scale, scale);

        p8g::stroke(0, 0, 0, 120);
        grid->DrawGrid(position);
        /*
        p8g::stroke(0, 0, 0, 255);
        DrawLines(lines, Point2D{ 0, 0 });

        p8g::stroke(255, 0, 0, 50);
        p8g::fill(255, 0, 0, 50);
        DrawCircle(robot_circle);

        p8g::strokeWeight(3);
        p8g::stroke(0, 0, 255, 255);
        p8g::line(robot_circle.center.x, robot_circle.center.y, mouse.x, mouse.y); */
    };

    static void KeyPressed () {
        static float angle = 0;
        if(eCtrl_Key == keyCode) {
            is_ctrl_pressed = true;
        }
        if(eRightArrow_Key == keyCode) {
            angle += 0.1;
            robot->SetAngle(angle);
            // torque += 100;
            // wheel->SetTorque(torque);
        }
        if(eLeftArrow_Key == keyCode) {
            angle -= 0.1;
            robot->SetAngle(angle);
            // torque -= 100;
            // wheel->SetTorque(torque);
        }
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
Point2D Draw::position;
vector<Line2D> Draw::lines;
bool Draw::is_ctrl_pressed;
bool Draw::is_right_button_pressed;
Point2D Draw::mouse;
float Draw::scale;
uint32_t Draw::window_width;
uint32_t Draw::window_height;
Circle2D Draw::robot_circle;
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