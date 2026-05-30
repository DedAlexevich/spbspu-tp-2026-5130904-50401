#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include <fstream>
#include "polygon.hpp"

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Couldnt open file\n";
    return 1;
  }
  namespace kuz = kuznetsov;
  std::vector< kuz::Polygon > data;

  while (!file.eof()) {
    using isi_t = std::istream_iterator< kuz::Polygon >;
    std::copy(isi_t{file}, isi_t{}, std::back_inserter(data));
    if (file.fail()) {
      file.clear();
      std::streamsize max = std::numeric_limits< std::streamsize >::max();
      file.ignore(max, '\n');
    }
  }

  using osi_t = std::ostream_iterator< kuz::Polygon >;
  std::copy(data.begin(), data.end(), osi_t{std::cout, "\n"});




}

