#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "polygon.hpp"
#include "commands.hpp"

namespace kuznetsov {
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
  std::vector< kuz::Polygon > data;
  kuz::readPoly(file, data);

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

void kuznetsov::runExecute(std::istream& in, CommandExecuter& ce)
{
  if (!(in >> ce)) {
    return;
  }
  runExecute(in, ce);
}


