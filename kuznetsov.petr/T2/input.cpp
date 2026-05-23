#include "input.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
kuznetsov::IOGuard::IOGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

kuznetsov::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.fill(fill_);
  s_.flags(fmt_);
}

bool kuznetsov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  bool f = lhs.key1 < rhs.key1;
  if (!f) {
    f = f || std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  f = f || lhs.key3.length() < rhs.key3.length();
  return f;
}

std::ostream& kuznetsov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IOGuard g(out);
  out << "(:key1 " << dest.key1 << "ull:";
  out << "key2 " << ConstCmpLsp{dest.key2} << ":";
  out << "key3 \"" << dest.key3 << "\":)";
  return out;
}

std::ostream& kuznetsov::operator<<(std::ostream& out, const ConstCmpLsp& dest)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IOGuard g(out);
  out << "#c(";
  out << std::fixed << std::setprecision(1) << dest.ref.real() << ' ' << dest.ref.imag() << ')';
  return out;
}

std::istream& kuznetsov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  char c = 0;
  in >> c;
  if (std::find(dest.expected.cbegin(), dest.expected.cend(), c) == dest.expected.cend()) {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream& kuznetsov::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  ull_t t = 0;
  using d_t = DelimiterIO;
  in >> t >> d_t{{'U', 'u'}} >> d_t{{'L', 'l'}} >> d_t{{'L', 'l'}};
  dest.ref = t;
  return in;
}

std::istream& kuznetsov::operator>>(std::istream& in, CmpLsp&& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  using d_t = DelimiterIO;
  double a = 0, b = 0;
  in >> d_t{{'#'}} >> d_t{{'c'}};
  in >> d_t{{'('}} >> a >> b >> d_t{{')'}};
  dest.ref = {a, b};
  return in;
}

std::istream& kuznetsov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  using d_t = DelimiterIO;
  return std::getline(in >> d_t{ {'"'} }, dest.ref, '"');
}

std::istream& kuznetsov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  std::string key1, key2, key3;
  std::vector< bool > beens(3);
  using d_t = DelimiterIO;
  in >> d_t{{'('}} >> d_t{{':'}};
  in >> key1 >> KeyValueInput{key1, beens, dest} >> d_t{{':'}};
  in >> key2 >> KeyValueInput{key2, beens, dest} >> d_t{{':'}};
  in >> key3 >> KeyValueInput{key3, beens, dest} >> d_t{{':'}};
  in >> d_t{{')'}};
  return in;
}

std::istream& kuznetsov::operator>>(std::istream& in, KeyValueInput inp)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IOGuard g(in);
  switch (inp.key[3]) {
  case '1':
    if (!inp.been[0]) {
      in >> UllIO{inp.ds.key1};
      inp.been[0] = true;
    } else {
      in.setstate(std::ios_base::failbit);
    }
    break;
  case '2':
    if (!inp.been[1]) {
      in >> CmpLsp{inp.ds.key2};
      inp.been[1] = true;
    } else {
      in.setstate(std::ios_base::failbit);
    }
    break;
  case '3':
    if (!inp.been[2]) {
      in >> StringIO{inp.ds.key3};
      inp.been[2] = true;
    } else {
      in.setstate(std::ios_base::failbit);
    }
    break;
  default:
    in.setstate(std::ios_base::failbit);
  }
  return in;
}
