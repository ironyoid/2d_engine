#include "parser.hpp"
#include "primitives.hpp"
#include <fstream>
#include <charconv>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

Parser::Parser(string path) : path(path){};

void Parser::WriteLine(Line2D line) {
    std::ofstream output(path.c_str(), std::ifstream::app);
    output << line.ToString() << std::endl;
    output.close();
}

std::pair<std::string, std::string> Parser::FindString(std::string s, std::string del) {
    std::string a, b;
    int end = s.find(del);
    if(std::string::npos != end) {
        a = s.substr(0, end);
        s.erase(s.begin(), s.begin() + end + del.length());
        b = s;
    } else {
        a = "";
        b = "";
    }
    return std::make_pair(a, b);
};

std::optional<Line2D> Parser::ReadLine(string line, uint32_t width, uint32_t height) {
    std::optional<Line2D> ret = std::nullopt;
    Line2D line2d;
    auto [a, b] = FindString(line, ";");
    auto [x1, y1] = FindString(a, ",");
    auto [x2, y2] = FindString(b, ",");
    if((x1 != "") && (y1 != "") && (x2 != "") && (y2 != "")) {
        line2d.a.x = std::stoi(x1.data());
        line2d.a.y = std::stoi(y1.data());
        line2d.b.x = std::stoi(x2.data());
        line2d.b.y = std::stoi(y2.data());
        ret = line2d;
    }
    return ret;
}

std::optional<Point2D> Parser::ReadLastLine(string line, uint32_t width, uint32_t height) {
    std::optional<Point2D> ret = std::nullopt;
    Point2D point;
    auto [x, y] = FindString(line, ",");
    if((x != "") && (y != "")) {
        point.x = std::stoi(x.data());
        point.y = std::stoi(y.data());
        ret = point;
    }
    return ret;
}

std::pair<std::vector<Line2D>, Point2D> Parser::ReadLines(uint32_t width, uint32_t height) {
    std::vector<Line2D> ret;
    Point2D robot_point;
    std::ifstream input(path.c_str(), std::ifstream::in);
    if(!input.fail()) {
        std::string line;
        for(; getline(input, line);) {
            auto res = ReadLine(line, width, height);
            if(res) {
                ret.push_back(res.value());
            }
        }
        auto ret_point = ReadLastLine(line, width, height);
        if(ret_point) {
            robot_point = ret_point.value();
        } else {
            robot_point.x = 0;
            robot_point.y = 0;
        }
    }
    return std::make_pair(ret, robot_point);
}

void Parser::WriteLines(std::vector<Line2D> lines, Point2D robot_point, Point2D position) {
    std::ofstream output(path.c_str(), std::ifstream::out | std::ofstream::trunc);
    for(auto n : lines) {
        output << n.ToStringNormalized(position) << std::endl;
    }
    output << robot_point.ToStringNormalized(position);
    output.close();
}