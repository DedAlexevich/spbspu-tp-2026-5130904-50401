#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <iosfwd>
#include <vector>
#include "polygon.hpp"

namespace kuznetsov {
  void area(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void max(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void min(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void count(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void same(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void rects(std::istream&, std::ostream&, const std::vector< Polygon >&);
}


#endif

