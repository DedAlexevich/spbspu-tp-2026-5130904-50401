#include "polygon.hpp"
#include <iostream>

class IOGuard {
public:
  explicit IOGuard(std::basic_ios< char >& s);
  ~IOGuard();
private:
  std::basic_ios< char >& s_;
  std::streamsize width_;
  std::streamsize precision_;
  std::basic_ios< char >::fmtflags fmt_;
  char fill_;
};

IOGuard::IOGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.fill(fill_);
  s_.flags(fmt_);

}
struct DelimiterIO {
  char expected;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  char c = 0;
  in >> c;
  if (c != dest.expected) {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream& kuznetsov::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  using d_t = DelimiterIO;
  size_t count = 0;
  int x = 0, y = 0;
  in >> count;
  std::vector< detail::Point > temp(count);
  for (size_t i = 0; i < count; ++i) {
    in >> d_t{'('} >> x >> d_t{';'} >> y >> d_t{')'};
    if (!in) {
      return in;
    }
    temp.push_back({x, y});
  }
  dest.points = std::move(temp);
  return in;
}

std::ostream& kuznetsov::operator<<(std::ostream& out, const Polygon& dest)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IOGuard g(out);
  out << dest.points.size();
  for (size_t i = 0; i < dest.points.size(); ++i) {
    out << ' ' << '(' << dest.points[i].x << ';' << dest.points[i].y << ')';
  }
  return out;
}



