#pragma once

#include <vector>
#include <cmath>

#include "types.h"

namespace geom {

const float EPS = 1e-4;

struct Point {
  f32 x, y;

  Point();
  Point(f32 x, f32 y);

  Point operator+(Point p) const;
  Point& operator+=(Point p);

  Point operator-(Point p) const;
  Point& operator-=(Point p);

  inline f64 abs2() const { return x * x + y * y; }
  inline f32 abs() const { return std::sqrt(abs2()); }

  inline Point normalize() {
    auto m = abs();
    x /= m; y /= m;
    return *this;
  }
};

struct Rect {
  f32 x, y, w, h;
};

struct Polygon {
  std::vector<Point> points;
};

bool point_inside_rect(Point p, Rect r);
bool point_inside_polygon(const Point p, const Polygon& polygon);

Point resolve_collision(Point p, Rect r);

Point min_abs_point(Point a, Point b);
Point min_abs_point(std::vector<Point> points);

/*
inline f64 cross(const Point a, const Point b) {
  return 
}
*/

} // namespace geometry
