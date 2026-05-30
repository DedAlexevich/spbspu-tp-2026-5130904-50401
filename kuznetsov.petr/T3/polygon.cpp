#include "polygon.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>

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

std::istream& kuznetsov::detail::operator>>(std::istream& in, detail::Point& dest)
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
std::ostream& kuznetsov::detail::operator<<(std::ostream& out, const detail::Point& dest)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IOGuard g(out);
  out << '(' << dest.x << ';' << dest.y << ')';
  return out;
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
  if (count == 0) {
    in.setstate(std::ios_base::failbit);
    return in;
  }
  std::vector< detail::Point > temp;
  temp.reserve(count);
  using isi_t = std::istream_iterator< detail::Point >;
  std::copy_n(isi_t{in}, count, std::back_inserter(temp));
  if (in && temp.size() == count) {
    dest.points = std::move(temp);
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



