#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

class File {
 public:
  explicit File(boost::filesystem::path);
  ~File();
  std::ifstream Read();
  std::ofstream Write();

 private:
  boost::filesystem::path _path;
};
