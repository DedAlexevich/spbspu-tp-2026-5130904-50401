#include "commands.hpp"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <vector>
#include "input.hpp"
#include "polygon.hpp"

using polygon_t = kuznetsov::Polygon;
using point_t = kuznetsov::detail::Point;

double kuznetsov::detail::fanTriArea(const polygon_t& p, size_t i)
{
  const point_t& a = p.points[0];
  const point_t& b = p.points[i];
  const point_t& c = p.points[i + 1];
  double bax = b.x - a.x;
  double cax = c.x - a.x;
  double cross = bax * (c.y - a.y) - cax * (b.y - a.y);
  return cross / 2.0;
}

double kuznetsov::detail::getArea(const polygon_t& p)
{
  if (p.points.size() < 3) {
    return 0.0;
  }
  using std::placeholders::_1;
  size_t triCount = p.points.size() - 2;
  std::vector< size_t > idx(triCount);
  std::iota(idx.begin(), idx.end(), 1);
  std::vector< double > areas;
  areas.reserve(triCount);
  std::transform(idx.begin(), idx.end(), std::back_inserter(areas), std::bind(fanTriArea, std::cref(p), _1));
  double signedArea = std::accumulate(areas.begin(), areas.end(), 0.0);
  return std::abs(signedArea);
}

bool kuznetsov::detail::hasEvenVertexes(const polygon_t& p)
{
  return p.points.size() % 2 == 0;
}

bool kuznetsov::detail::hasOddVertexes(const polygon_t& p)
{
  return p.points.size() % 2 == 1;
}

bool kuznetsov::detail::hasNVertexes(size_t n, const polygon_t& p)
{
  return p.points.size() == n;
}

double kuznetsov::detail::sumAreaIf(const std::vector< polygon_t >& data, std::function< bool(const polygon_t&) > pred)
{
  std::vector< polygon_t > approved;
  std::copy_if(data.begin(), data.end(), std::back_inserter(approved), pred);
  std::vector< double > areas(approved.size());
  std::transform(approved.begin(), approved.end(), areas.begin(), getArea);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

double kuznetsov::detail::sumAllArea(const std::vector< polygon_t >& data)
{
  std::vector< double > areas(data.size());
  std::transform(data.begin(), data.end(), areas.begin(), getArea);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

void kuznetsov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& ps)
{
  std::string param;
  in >> param;

  using std::placeholders::_1;
  IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  if (param == "EVEN") {
    out << detail::sumAreaIf(ps, detail::hasEvenVertexes) << '\n';
  } else if (param == "ODD") {
    out << detail::sumAreaIf(ps, detail::hasOddVertexes) << '\n';
  } else if (param == "MEAN") {
    if (ps.empty()) {
      throw std::logic_error("Need one polygon as minimum");
    }
    out << detail::sumAllArea(ps) / ps.size() << '\n';
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::logic_error("Invalid argument");
    } else {
      out << detail::sumAreaIf(ps, std::bind(detail::hasNVertexes, n, _1)) << '\n';
    }
  }
}

template< class CMP >
void kuznetsov::detail::finder(std::istream& in, std::ostream& out, const std::vector< polygon_t >& ps, CMP cmp)
{
  std::string param;
  in >> param;
  using std::placeholders::_1;
  if (ps.empty()) {
    throw std::logic_error("Empty polygons");
  }
  IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  if (param == "AREA") {
    std::vector< double > areas;
    std::transform(ps.begin(), ps.end(), std::back_inserter(areas), getArea);
    out << *(std::max_element(areas.begin(), areas.end(), cmp)) << '\n';
  } else if (param == "VERTEXES") {
    std::vector< size_t > vrts;

    auto getSize = std::bind(&std::vector< point_t >::size,
        std::bind(&Polygon::points, _1));

    std::transform(ps.begin(), ps.end(), std::back_inserter(vrts), getSize);
    size_t res = *(std::max_element(vrts.begin(), vrts.end(), cmp));
    out << res << '\n';
  } else {
    throw std::logic_error("Unknown argument");

  }
}

void kuznetsov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& ps)
{
  detail::finder(in, out, ps, std::less<>{});
}

void kuznetsov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& ps)
{
  detail::finder(in, out, ps, std::greater<>{});
}

void kuznetsov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& ps)
{
  std::string param;
  in >> param;
  using std::placeholders::_1;

  if (param == "EVEN") {
    out << std::count_if(ps.begin(), ps.end(), detail::hasEvenVertexes) << '\n';
  } else if (param == "ODD") {
    out << std::count_if(ps.begin(), ps.end(), detail::hasOddVertexes) << '\n';
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::logic_error("Invalid argument");
    }
    out << std::count_if(ps.begin(), ps.end(), std::bind(detail::hasNVertexes, n, _1)) << '\n';
  }
}

bool kuznetsov::detail::pointLess(const point_t& a, const point_t& b)
{
  return a.x != b.x ? a.x < b.x : a.y < b.y;
}

point_t kuznetsov::detail:: subPoint(const point_t& base, const point_t& p)
{
  return point_t{p.x - base.x, p.y - base.y};
}

bool kuznetsov::detail::pointEq(const point_t& a, const point_t& b)
{
  return a.x == b.x && a.y == b.y;
}

std::vector< point_t > kuznetsov::detail::normalize(const polygon_t& poly)
{
  std::vector< point_t > v = poly.points;
  point_t base = *std::min_element(v.begin(), v.end(), pointLess);
  using std::placeholders::_1;
  std::transform(v.begin(), v.end(), v.begin(), std::bind(subPoint, std::cref(base), _1));
  std::sort(v.begin(), v.end(), pointLess);
  return v;
}

bool kuznetsov::detail::sameShape(const std::vector< point_t >& target, const polygon_t& poly)
{
  if (target.size() != poly.points.size()) {
    return false;
  }
  std::vector< point_t > nb = normalize(poly);
  return std::equal(target.begin(), target.end(), nb.begin(), pointEq);
}

bool kuznetsov::detail::atLineEnd(std::istream& in)
{
  int ch = in.peek();
  if (ch == ' ' || ch == '\t' || ch == '\r') {
    in.get();
    return atLineEnd(in);
  }
  return ch == '\n' || ch == EOF;
}

void kuznetsov::same(std::istream& in, std::ostream& out, const std::vector< Polygon >& p)
{
  polygon_t poly;
  in >> poly;
  if (!in || poly.points.size() < 3 || !detail::atLineEnd(in)) {
    throw std::logic_error("Bad polygon");
  }
  std::vector< point_t > norm = detail::normalize(poly);
  using std::placeholders::_1;
  out << std::count_if(p.begin(), p.end(), std::bind(detail::sameShape, std::cref(norm), _1)) << '\n';
}

long kuznetsov::detail::rectAngle(const point_t& a, const point_t& b, const point_t& c)
{
  long v1x = b.x - a.x;
  long v1y = b.y - a.y;
  long v2x = c.x - b.x;
  long v2y = c.y - b.y;
  return v1x * v2x + v1y * v2y;
}

bool kuznetsov::detail::isRect(const polygon_t& p)
{
  if (p.points.size() != 4) {
    return false;
  }
  const std::vector< point_t >& v = p.points;
  bool f = rectAngle(v[3], v[0], v[1]) == 0;
  f = f && rectAngle(v[0], v[1], v[2]) == 0;
  f = f && rectAngle(v[1], v[2], v[3]) == 0;
  f = f && rectAngle(v[2], v[3], v[0]) == 0;
  return f;
}

void kuznetsov::rects(std::istream&, std::ostream& out, const std::vector< Polygon >& ps)
{
  out << std::count_if(ps.begin(), ps.end(), detail::isRect) << '\n';
}

std::istream& kuznetsov::operator>>(std::istream& in, CommandExecuter& cmd)
{
  std::istream::sentry s(in);
  if(!s) {
    return in;
  }
  IOGuard g(in);
  std::string cm;
  in >> cm;
  try {
    cmd.cmds.at(cm)(in, cmd.out, cmd.ps);
  } catch(...) {
    cmd.out << "<INVALID COMMAND>\n";
    in.clear();
    std::streamsize max = std::numeric_limits< std::streamsize >::max();
    in.ignore(max, '\n');
  }
  return in;
}


