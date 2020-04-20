#include "geom.h"

namespace geom {

Point::Point(): x(0), y(0) {}
Point::Point(float x, float y): x(x), y(y) {}

Point Point::operator+(Point p) const { return Point(x+p.x, y+p.y); }
Point& Point::operator+=(Point p) { *this = Point(x+p.x, y+p.y); return *this; }

Point Point::operator-() const { return Point(-x, -y); }
Point Point::operator-(Point p) const { return Point(x-p.x, y-p.y); }
Point& Point::operator-=(Point p) { *this = Point(x-p.x, y-p.y); return *this; }

Point Point::operator*(f32 k) const { return Point(x*k, y*k); }

f32 Point::operator%(Point p) const { return x*p.y - y*p.x; }

bool point_inside_rect(Point p, Rect r) {
    return p.x <= r.x + r.w + EPS and
           p.x >= r.x - EPS and
           p.y <= r.y + r.h + EPS and
           p.y >= r.y - EPS;
}

// Assumes that the polygon is given in counterclockwise order.
// It doesn't check if the polygon is actually convex.
bool point_inside_convex_polygon(Point p, Polygon polygon) {
    int n = polygon.size();
    for(int i = 0; i < n; i++) {
        if ((polygon[(i+1)%n] - polygon[i]) % (p - polygon[i]) < -EPS)
            return false;
    }
    return true;
}

Point resolve_collision(Point p, Rect r) {
  Point out_right = { r.x + r.w - p.x, 0 };
  Point out_left  = { r.x - p.x,       0 };
  Point out_up    = { 0, r.y + r.h - p.y };
  Point out_down  = { 0, r.y - p.y       };

  Point diff = min_abs_point({out_right, out_left, out_up, out_down});
  return p + diff;
}

Point resolve_collision(Point p, Polygon polygon) {
  int n = polygon.size();
  std::vector<Point> out;
  for (int i = 0; i < n; i++) {
    Point edge = polygon[(i+1)%n] - polygon[i];
    Point b = p - polygon[i];
    b.normalize();

    Point out_dir = {-edge.y, edge.x};
    out_dir.normalize();

    f32 magnitude = edge % b;
    out.push_back(- out_dir * magnitude);
  }
  Point diff = min_abs_point(out);
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
