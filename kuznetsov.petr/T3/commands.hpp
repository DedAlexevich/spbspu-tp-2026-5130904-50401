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
}


#endif

