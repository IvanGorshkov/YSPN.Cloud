#pragma once
#include <vector>
#include <string>
#include <fstream>

class File {
 public:
  File(std::string);
  ~File();
  std::ifstream Read();
  std::ofstream Write();

 private:
  std::string _path;
};
