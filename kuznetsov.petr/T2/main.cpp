#include <iostream>
#include <iterator>
#include <algorithm>
#include <complex>

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
    double cmp1 = std::sqrt(std::pow(lhs.key2.real(), 2) + std::pow(lhs.key2.imag(), 2));
    double cmp2 = std::sqrt(std::pow(rhs.key2.real(), 2) + std::pow(rhs.key2.imag(), 2));
    f = f || cmp1 < cmp2;
  }
  f = f || lhs.key3.length() < rhs.key3.length();
  return f;
}

