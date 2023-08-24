#pragma once
#include <iostream>
#include <string>
#include <stdint.h>
#include "points.hpp"
#include <optional>
#include <sys/types.h>
#include <vector>
#include <utility>

class Parser
{
    string path;
    std::pair<std::string, std::string> FindString (std::string s, std::string del);

   public:
    Parser(string path);
    void WriteLine (Line2D line);
    void WriteLines (std::vector<Line2D> lines, Point2D robot_point, Point2D position);
    std::optional<Line2D> ReadLine (string line, uint32_t width, uint32_t height);
    std::optional<Point2D> ReadLastLine (string line, uint32_t width, uint32_t height);
    std::pair<std::vector<Line2D>, Point2D> ReadLines (uint32_t width, uint32_t height);
};