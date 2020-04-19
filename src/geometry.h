#pragma once

namespace geometry {

struct Point {
    float x, y;

    Point();
    Point(float x, float y);

    Point operator+(Point p) const;
    Point& operator+=(Point p);

    Point operator-(Point p) const;
};

struct Rect {
    int x, y;
    int width, height;
};

bool point_inside_Rect(Point p, Rect r);

} // namespace geometry