#pragma once
#include "stdlib.h"
#include <vector>
#include <string>
#include "FileChunk.h"
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
