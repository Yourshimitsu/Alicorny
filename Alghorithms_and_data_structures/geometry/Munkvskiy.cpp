#include <cmath>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::min;
using std::sqrt;
using std::vector;

const long double kEpsilon = 1e-6;

struct Point {
  long long x;
  long long y;
};

std::istream& operator>>(std::istream& os, Point& obj) {
  os >> obj.x >> obj.y;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Point& obj) {
  os << obj.x << ' ' << obj.y;
  return os;
}

struct Vector {
  long long x;
  long long y;

  Vector(Point aa, Point bb) : x{bb.x - aa.x}, y{bb.y - aa.y} {};

  Vector(long long x1, long long y1) : x{x1}, y{y1} {};

  Vector(long long x1, long long y1, long long x2, long long y2) {
    x = x2 - x1;
    y = y2 - y1;
  }

  long double Abs() const { return std::sqrt(x * x + y * y); }

  Vector operator+(const Vector& vector) const {
    return Vector(vector.x + this->x, vector.y + this->y);
  }

  long long operator*(const Vector& vector) const {
    return vector.x * this->x + vector.y * this->y;
  }
};

Vector operator%(const Vector& vector1, const Vector& vector2) {
  return Vector(vector1.x * vector2.y, -vector1.y * vector2.x);
}

long long operator/(const Vector& vector1, const Vector& vector2) {
  return vector1.x * vector2.y - vector1.y * vector2.x;
}

std::ostream& operator<<(std::ostream& os, const Vector& obj) {
  os << obj.x << ' ' << obj.y;
  return os;
}

struct Line {
  Point point;
  Vector dir;

  Point start() {
    return point;
  }

  Point end() {
    return Point{point.x + dir.x, point.y + dir.y};
  }

  Line(const Point& aa, const Point& bb) : point(aa), dir(aa, bb) {};
};

struct Polygon {
  vector<Line> sides;

  long long size() {
    return sides.size();
  }

  void add(const Line& vec) {
    sides.push_back(vec);
  }

  void add(const Point& a, const Point& b) {
    sides.push_back(Line(a, b));
  }
};

long double Angle(const Vector& a, const Vector& b) {
  long double angle = acos((a * b) / a.Abs() / b.Abs());
  if (a / b < 0) {
    angle *= -1;
  }
  return angle;
}

long double Angle(const Vector& a, const Vector& b, bool abs) {
  long double angle = acos((a * b) / a.Abs() / b.Abs());
  if (a / b < 0) {
    angle = -angle + 1000;
  }
  return angle;
}

bool Inside(const Polygon& vec, const Point& point) {
  long double ch = 0;
  for (auto line : vec.sides) {
    Point aa = line.start();
    Point bb = line.end();
    if (aa.x == point.x and aa.y == point.y) {
      return true;
    }
    if (bb.x == point.x and bb.y == point.y) {
      return true;
    }
    Vector pa = Vector(point, aa);
    Vector pb = Vector(point, bb);
    if ((pa.x * pb.x < 0 or pa.y * pb.y < 0) and (pa.x * pb.y == pa.y * pb.x)) {
      return true;
    }
    ch += Angle(pa, pb);
  }
  return std::abs(ch) > kEpsilon;
}

void Input(Polygon& a, int n, bool reverse = false) {
  vector<Point> points(n + 1);
  int start = 0;
  for (int i = 0; i < n; ++i) {
    cin >> points[i];
    if (reverse) {
      points[i].x *= -1;
      points[i].y *= -1;
    }
    if (points[i].y < points[start].y or (points[i].y == points[start].y and points[i].x < points[start].x)) {
      start = i;
    }
  }
  points[n] = points[0];
  int i = (start + 1) % n;
  while (i != start) {
    a.add(points[(i + n - 1) % n], points[i]);
    ++i;
    i %= n;
  }
  a.add(points[(i + n - 1) % n], points[i]);
}

Polygon Minkovskiy(Polygon& airport, Polygon& cloud) {
  vector<Point> points;
  Point point1, point2;
  point1 = airport.sides[0].start();
  point2 = cloud.sides[0].start();
  Point point = Point{point1.x + point2.x, point1.y + point2.y};
  points.push_back(point);
  int i = 0, j = 0;
  int n = airport.size(), m = cloud.size();
  Vector vec(1, 0);
  while (i < n or j < m) {
    if (i == n) {
      point.x += cloud.sides[j].dir.x;
      point.y += cloud.sides[j].dir.y;
      points.push_back(point);
      ++j;
    } else if (j == m) {
      point.x += airport.sides[i].dir.x;
      point.y += airport.sides[i].dir.y;
      points.push_back(point);
      ++i;
    } else {
      if (Angle(vec, airport.sides[i].dir, true) <= Angle(vec, cloud.sides[j].dir, true)) {
        point.x += airport.sides[i].dir.x;
        point.y += airport.sides[i].dir.y;
        points.push_back(point);
        ++i;
      } else {
        point.x += cloud.sides[j].dir.x;
        point.y += cloud.sides[j].dir.y;
        points.push_back(point);
        ++j;
      }
    }
  }
  Polygon ans;
  for (int i = 0; i < points.size() - 1; ++i) {
    ans.add(points[i], points[i + 1]);
  }
  return ans;
}

long double Dist(const Point& cc, const Line& ab) {
  Vector ac(ab.point, cc);
  if (ac / ab.dir >= 0) {
    return ac / ab.dir / (ab.dir.Abs());
  }
  return -(ac / ab.dir / (ab.dir.Abs()));
}

long double Dist(const Point& aa, const Point& bb, const Point& cc) {
  long double parametr;
  Vector ca(cc, aa);
  Vector ab(aa, bb);
  parametr = -(ab * ca) / static_cast<long double>(ab * ab);
  long double min_dist = std::min(Vector(cc, bb).Abs(), ca.Abs());
  if (ab * ca <= 0 and - (ab * ca) <= ab * ab) {
    return Dist(cc, Line(aa, bb));
  } else {
    return min_dist;
  }
}

int main() {
  int n, m;
  Polygon cloud, airport;
  cin >> n >> m;
  Input(airport, n);
  Input(cloud, m, true);
  const long long kMagicNumber = 50;
  cout.precision(kMagicNumber);
  Point point{0, 0};
  Polygon polygon = Minkovskiy(airport, cloud);
  if (Inside(polygon, point)) {
    long double ans = 1e20;
    ans = Dist(polygon.sides[0].start(), polygon.sides[0].end(), point);
    for (auto line : polygon.sides) {
      if (Dist(line.start(), line.end(), point) < ans) {
        ans = Dist(line.start(), line.end(), point);
      }
    }
    cout << std::max(static_cast<long double>(0), ans - 60);
  } else {
    cout << 0;
  }
}
