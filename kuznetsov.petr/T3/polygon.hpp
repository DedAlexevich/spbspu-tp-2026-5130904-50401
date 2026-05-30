#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iosfwd>

namespace kuznetsov {
  namespace detail {
    struct Point {
      int x, y;
    };
  }

  struct Polygon {
    std::vector< detail::Point > points;
  };

  std::istream& operator>>(std::istream& in, Polygon& dest);
  std::ostream& operator<<(std::ostream& out, const Polygon& dest);

}
#endif

