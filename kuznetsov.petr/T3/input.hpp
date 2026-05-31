#ifndef INPUT_HPP
#define INPUT_HPP
#include <iosfwd>
#include <iomanip>

namespace kuznetsov {

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

  struct DelimiterIO {
    char expected;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

}

#endif

