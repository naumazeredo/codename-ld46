#pragma once

#include <vector>

namespace geometry {

const float EPS = 1e-4;

struct Point {
    float x, y;

    Point();
    Point(float x, float y);

    Point operator+(Point p) const;
    Point& operator+=(Point p);

    Point operator-(Point p) const;
    Point& operator-=(Point p);

    float operator ~() const;

    Point& normalize();
};

struct Rect {
    Point center;
    float width, height;
};

bool point_inside_Rect(Point p, Rect r);

Point min_abs_point(Point a, Point b);
Point min_abs_point(std::vector<Point> points);

} // namespace geometry