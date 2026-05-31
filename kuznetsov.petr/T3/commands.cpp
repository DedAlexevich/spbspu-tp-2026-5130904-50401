#include "commands.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include "polygon.hpp"

using polygon_t = kuznetsov::Polygon;
using point_t = kuznetsov::detail::Point;

double fanTriArea(const polygon_t& p, size_t i)
{
  const point_t& a = p.points[0];
  const point_t& b = p.points[i];
  const point_t& c = p.points[i + 1];
  double bax = b.x - a.x;
  double cax = c.x - a.x;
  double cross = bax * (c.y - a.y) - cax * (b.y - a.y);
  return cross / 2.0;
}

double getArea(const polygon_t& p)
{
  if (p.points.size() < 3) {
    return 0.0;
  }
  using plus = std::plus< double >;
  using std::placeholders::_1;
  using std::placeholders::_2;
  size_t triCount = p.points.size() - 2;
  std::vector< size_t > idx(triCount);
  std::iota(idx.begin(), idx.end(), 1);
  double signedArea = std::transform_reduce(idx.begin(), idx.end(), 0.0, plus{}, std::bind(fanTriArea, std::cref(p), _1));
  return std::abs(signedArea);
}

bool hasEvenVertexes(const polygon_t& p)
{
  return p.points.size() % 2 == 0;
}

bool hasOddVertexes(const polygon_t& p)
{
  return p.points.size() % 2 == 1;
}

bool hasNVertexes(size_t n, const polygon_t& p)
{
  return p.points.size() == n;
}

double sumAreaIf(const std::vector< polygon_t >& data, std::function< bool(const polygon_t&) > pred)
{
  using plus = std::plus< double >;
  std::vector< polygon_t > approved;
  std::copy_if(data.begin(), data.end(), std::back_inserter(approved), pred);
  return std::transform_reduce(approved.begin(), approved.end(), 0.0, plus{}, getArea);
}

double sumAllArea(const std::vector< polygon_t >& data)
{
  using plus = std::plus< double >;
  return std::transform_reduce(data.begin(), data.end(), 0.0, plus{}, getArea);
}

void kuznetsov::area(std::istream& in, std::ostream& out, std::vector< Polygon >& ps)
{
  std::string param;
  in >> param;

  using std::placeholders::_1;
  std::ios_base::fmtflags fmt = out.flags();
  out << std::fixed << std::setprecision(1);

  if (param == "EVEN") {
    out << sumAreaIf(ps, hasEvenVertexes) << '\n';
  } else if (param == "ODD") {
    out << sumAreaIf(ps, hasOddVertexes) << '\n';
  } else if (param == "MEAN") {
    if (ps.empty()) {
      out.flags(fmt);
      throw std::logic_error("Requared one polygon as minimum");
    }
    out << sumAllArea(ps) / ps.size() << '\n';
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      out << 0.0 << '\n';
    } else {
      out << sumAreaIf(ps, std::bind(hasNVertexes, n, _1)) << '\n';
    }
  }
  out.flags(fmt);
}

void kuznetsov::max(std::istream& in, std::ostream& out, std::vector< Polygon >& ps)
{
  std::string param;
  in >> param;
  using std::placeholders::_1;
  std::ios_base::fmtflags fmt = out.flags();
  out << std::fixed << std::setprecision(1);
  if (ps.empty()) {

    throw std::logic_error("Empty polygons");
  }
  if (param == "AREA") {
    std::vector< double > areas;
    std::transform(ps.begin(), ps.end(), std::back_inserter(areas), getArea);
    out << *(std::max_element(areas.begin(), areas.end())) << '\n';
  } else {
    std::vector< size_t > vrts;
    auto getSize = std::bind(&std::vector< detail::Point >::size,
        std::bind(&kuznetsov::Polygon::points, _1));
    std::transform(ps.begin(), ps.end(), std::back_inserter(vrts), getSize);
    size_t res = *(std::max_element(vrts.begin(), vrts.end()));
    out << res << '\n';
  }
}

