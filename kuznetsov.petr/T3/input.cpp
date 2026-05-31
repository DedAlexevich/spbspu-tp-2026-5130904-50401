#include "input.hpp"
#include <iostream>

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

std::istream& kuznetsov::operator>>(std::istream& in, DelimiterIO&& dest)
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

