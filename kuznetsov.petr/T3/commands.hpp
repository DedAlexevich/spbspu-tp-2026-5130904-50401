#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <iosfwd>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include "polygon.hpp"

namespace kuznetsov {
  using polygons_t = std::vector< Polygon >;
  using cmd_t = std::function< void(std::istream&, std::ostream&, const polygons_t&) >;
  struct CommandExecuter {
    std::unordered_map<std::string, cmd_t > cmds;
    std::ostream& out;
    const polygons_t& ps;
  };
  std::istream& operator>>(std::istream&, CommandExecuter& cmd);
  void area(std::istream&, std::ostream&, const polygons_t&);
  void max(std::istream&, std::ostream&, const polygons_t&);
  void min(std::istream&, std::ostream&, const polygons_t&);
  void count(std::istream&, std::ostream&, const polygons_t&);
  void same(std::istream&, std::ostream&, const polygons_t&);
  void rects(std::istream&, std::ostream&, const polygons_t&);

  using polygon_t = Polygon;
  using point_t = detail::Point;
  namespace detail {
    double fanTriArea(const polygon_t& p, size_t i);
    double getArea(const polygon_t& p);
    bool hasEvenVertexes(const polygon_t& p);
    bool hasOddVertexes(const polygon_t& p);
    bool hasNVertexes(size_t n, const polygon_t& p);
    double sumAreaIf(const std::vector< polygon_t >& data, std::function< bool(const polygon_t&) > pred);
    double sumAllArea(const std::vector< polygon_t >& data);
    template< class CMP >
    void finder(std::istream& in, std::ostream& out, const std::vector< polygon_t >& ps, CMP cmp);
    bool pointLess(const point_t& a, const point_t& b);
    point_t subPoint(const point_t& base, const point_t& p);
    bool pointEq(const point_t& a, const point_t& b);
    std::vector< point_t > normalize(const polygon_t& poly);
    bool sameShape(const std::vector< point_t >& target, const polygon_t& poly);
    bool atLineEnd(std::istream& in);
    long rectAngle(const point_t& a, const point_t& b, const point_t& c);
    bool isRect(const polygon_t& p);
  }

}


#endif

