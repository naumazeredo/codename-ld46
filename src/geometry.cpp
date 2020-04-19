#include "geometry.h"

namespace geometry {

Point::Point(): x(0), y(0) {}
Point::Point(float x, float y): x(x), y(y) {}

Point Point::operator+(Point p) const { return Point(x+p.x, y+p.y); }
Point& Point::operator+=(Point p) { *this = Point(x+p.x, y+p.y); return *this; }

Point Point::operator-(Point p) const { return Point(x-p.x, y-p.y); }

bool point_inside_Rect(Point p, Rect r) {
    return p.x <= r.x + r.width and
           p.x >= r.x - r.width and
           p.y <= r.y + r.height and
           p.y >= r.y - r.height;
}

} // namespace geometry