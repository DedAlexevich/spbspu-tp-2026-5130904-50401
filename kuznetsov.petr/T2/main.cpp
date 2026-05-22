#include <iostream>
#include <iterator>
#include <algorithm>
#include <complex>
#include <iomanip>

namespace kuznetsov {
  using ull_t = unsigned long long;

  struct UllIO {
    ull_t& ref;
  };

  struct CmpLsp {
    std::complex< double > ref;
  };

  struct delimiter_t {
    char expected;
  };

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

  struct DataStruct {
    ull_t key1;
    std::complex< double > key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, CmpLsp&& dest);
  std::istream& operator>>(std::istream& in, delimiter_t&& dest);
  std::istream& operator>>(std::istream& in, IOGuard&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const CmpLsp& dest);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

int main()
{
  namespace kuz = kuznetsov;
  std::vector< kuz::DataStruct > data;
  {
    using isi_t = std::istream_iterator< kuz::DataStruct >;
    std::copy(isi_t{std::cin}, isi_t{}, std::back_inserter(data));
  }
  std::sort(data.begin(), data.end(), std::less < kuz::DataStruct >{});
  {
    using osi_t = std::ostream_iterator< kuz::DataStruct >;
    std::copy(data.begin(), data.end(), osi_t{std::cout, "\n"});
  }
}

kuznetsov::IOGuard::IOGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
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
  out << "key2 " << CmpLsp{dest.key2} << ":";
  out << "key3 \"" << dest.key3 << "\":)";
  return out;
}

std::ostream& kuznetsov::operator<<(std::ostream& out, const CmpLsp& dest)
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

