#ifndef SPBSPU_TP_2026_5130904_50401_INPUT_H
#define SPBSPU_TP_2026_5130904_50401_INPUT_H
#include <complex>
#include <iosfwd>
#include <vector>
namespace kuznetsov {
  using ull_t = unsigned long long;

  struct UllIO {
    ull_t& ref;
  };

  struct CmpLsp {
    std::complex< double >& ref;
  };

  struct ConstCmpLsp {
    const std::complex<double>& ref;
  };

  struct DelimiterIO {
    std::vector< char > expected;
  };

  struct StringIO {
    std::string& ref;
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

  struct KeyValueInput {
    std::string key;
    std::vector< bool >& been;
    DataStruct& ds;
  };

  std::istream& operator>>(std::istream& in, KeyValueInput inp);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, CmpLsp&& dest);
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const ConstCmpLsp& dest);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}
#endif

