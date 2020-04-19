#include "geom.h"

namespace geom{

Point::Point(): x(0), y(0) {}
Point::Point(float x, float y): x(x), y(y) {}

Point Point::operator+(Point p) const { return Point(x+p.x, y+p.y); }
Point& Point::operator+=(Point p) { *this = Point(x+p.x, y+p.y); return *this; }

Point Point::operator-(Point p) const { return Point(x-p.x, y-p.y); }
Point& Point::operator-=(Point p) { *this = Point(x-p.x, y-p.y); return *this; }

bool point_inside_rect(Point p, Rect r) {
    return p.x <= r.center.x + r.width / 2 and
           p.x >= r.center.x - r.width / 2 and
           p.y <= r.center.y + r.height / 2 and
           p.y >= r.center.y - r.height / 2;
}

/*
bool point_inside_polygon(const Point p, const Polygon& polygon) {
}
*/

Point min_abs_point(Point a, Point b) { return a.abs() - b.abs() > EPS ? b : a; }

Point min_abs_point(std::vector<Point> points) {
    if (points.empty()) return Point();
    Point ans = points[0];
    for(auto& p : points) {
        ans = min_abs_point(ans, p);
    }
    return ans;
}

} // namespace geometry
