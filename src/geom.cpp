#include "geom.h"

namespace geom {

Point::Point(): x(0), y(0) {}
Point::Point(float x, float y): x(x), y(y) {}

Point Point::operator+(Point p) const { return Point(x+p.x, y+p.y); }
Point& Point::operator+=(Point p) { *this = Point(x+p.x, y+p.y); return *this; }

Point Point::operator-(Point p) const { return Point(x-p.x, y-p.y); }
Point& Point::operator-=(Point p) { *this = Point(x-p.x, y-p.y); return *this; }

bool point_inside_rect(Point p, Rect r) {
    return p.x <= r.x + r.w + EPS and
           p.x >= r.x - EPS and
           p.y <= r.y + r.h + EPS and
           p.y >= r.y - EPS;
}

/*
bool point_inside_polygon(const Point p, const Polygon& polygon) {

}
*/

Point resolve_collision(Point p, Rect r) {
  Point out_right = { r.x + r.w - p.x, 0 };
  Point out_left  = { r.x - p.x,       0 };
  Point out_up    = { 0, r.y + r.h - p.y };
  Point out_down  = { 0, r.y - p.y       };

  Point diff = min_abs_point({out_right, out_left, out_up, out_down});
  return p + diff;
}

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
