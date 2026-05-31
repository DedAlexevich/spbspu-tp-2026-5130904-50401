#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "polygon.hpp"

namespace kuznetsov {
  bool isNotEmpty(Polygon& p);
}

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
  std::vector< kuz::Polygon > raw;
  using isi_t = std::istream_iterator< kuz::Polygon >;
  std::copy(isi_t{file}, isi_t{}, std::back_inserter(raw));

  std::vector< kuz::Polygon > data;
  std::copy_if(raw.begin(), raw.end(), std::back_inserter(data), kuz::isNotEmpty);
  raw.clear();

  using osi_t = std::ostream_iterator< kuz::Polygon >;
  std::copy(data.begin(), data.end(), osi_t{std::cout, "\n"});
}

bool kuznetsov::isNotEmpty(Polygon& p)
{
  return p.points.size();
}



