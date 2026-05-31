#include <iostream>
#include "input.hpp"
#include <iterator>
#include <limits>
#include <algorithm>
#include <vector>

int main()
{
  namespace kuz = kuznetsov;
  std::vector< kuz::DataStruct > data;
  {
    while (!std::cin.eof()) {
      using isi_t = std::istream_iterator< kuz::DataStruct >;
      std::copy(isi_t{std::cin}, isi_t{}, std::back_inserter(data));
      if (std::cin.fail()) {
        std::cin.clear();
        std::streamsize max = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(max, '\n');
      }
    }
  }
  std::sort(data.begin(), data.end(), std::less < kuz::DataStruct >{});
  {
    using osi_t = std::ostream_iterator< kuz::DataStruct >;
    std::copy(data.begin(), data.end(), osi_t{std::cout, "\n"});
  }
}

