#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iosfwd>

namespace kuznetsov {
  namespace detail {
    struct Point {
      int x, y;
    };
    std::istream& operator>>(std::istream& in, detail::Point& dest);
    std::ostream& operator<<(std::ostream& out, const detail::Point& dest);
  }

  struct Polygon {
    std::vector< detail::Point > points;
  };
  void readPoly(std::istream& in, std::vector< Polygon >& dst);
  std::istream& operator>>(std::istream& in, Polygon& dest);
  std::ostream& operator<<(std::ostream& out, const Polygon& dest);

}
#endif

