#include "polygon.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>

#include "commands.hpp"
#include "input.hpp"

std::istream& kuznetsov::detail::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  using d_t = DelimiterIO;
  detail::Point temp{0, 0};
  in >> d_t{'('} >> temp.x >> d_t{';'} >> temp.y >> d_t{')'};
  if(in) {
    dest = temp;
  }
  return in;
}
std::ostream& kuznetsov::detail::operator<<(std::ostream& out, const Point& dest)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IOGuard g(out);
  out << '(' << dest.x << ';' << dest.y << ')';
  return out;
}

void clearIstream(std::istream& in, kuznetsov::Polygon& p)
{
  in.clear();
  std::streamsize max = std::numeric_limits< std::streamsize >::max();
  in.ignore(max, '\n');
  p.points.clear();
}

std::istream& kuznetsov::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  size_t count = 0;
  in >> count;
  if (!in || count < 3) {
    in.setstate(std::ios_base::failbit);
    dest.points.clear();
    return in;
  }
  std::vector< detail::Point > temp;
  temp.reserve(count);
  using isi_t = std::istream_iterator< detail::Point >;
  std::copy_n(isi_t{in}, count, std::back_inserter(temp));
  if (in && temp.size() == count) {
    dest.points = std::move(temp);
  } else {
    in.setstate(std::ios_base::failbit);
    dest.points.clear();
  }
  return in;
}

std::ostream& kuznetsov::operator<<(std::ostream& out, const Polygon& dest)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IOGuard g(out);
  size_t c = dest.points.size();
  out << c << ' ';
  using osi_t = std::ostream_iterator< detail::Point >;
  std::copy_n(dest.points.begin(), c - 1, osi_t{out, " "});
  out << dest.points[c - 1];
  return out;
}

void kuznetsov::readPoly(std::istream& in, polygons_t& dst)
{
  Polygon p;
  in >> p;
  if (in.eof() && p.points.empty()) {
    return;
  }
  if (in.fail()) {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  } else if (!p.points.empty()) {
    dst.push_back(p);
  }
  if (!in.eof()) {
    readPoly(in, dst);
  }
}



