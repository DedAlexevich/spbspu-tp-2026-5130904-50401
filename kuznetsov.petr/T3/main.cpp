#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "polygon.hpp"
#include "commands.hpp"

namespace kuznetsov {
  bool isNotEmpty(Polygon& p);
  void runExecute(std::istream&, CommandExecuter&);
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
  std::unordered_map< std::string, kuz::cmd_t > cmds;
  cmds["AREA"] = kuz::area;
  cmds["MAX"] = kuz::max;
  cmds["MIN"] = kuz::min;
  cmds["COUNT"] = kuz::count;
  cmds["SAME"] = kuz::same;
  cmds["RECTS"] = kuz::rects;
  kuz::CommandExecuter ce{cmds, std::cout, data};
  kuz::runExecute(std::cin, ce);
}

bool kuznetsov::isNotEmpty(Polygon& p)
{
  return p.points.size();
}

void kuznetsov::runExecute(std::istream& in, CommandExecuter& ce)
{
  if (!(in >> ce)) {
    return;
  }
  runExecute(in, ce);
}


