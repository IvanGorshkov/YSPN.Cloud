#include "File.h"

File::File(boost::filesystem::path path) : _path(path) {
  if(!boost::filesystem::exists(path.parent_path())){
    boost::filesystem::create_directories(path.parent_path());
  }
}

std::ifstream File::Read() {
  std::ifstream fileStream;
  fileStream.open(_path.c_str(), std::ios::in | std::ios::binary);
  return fileStream;
}

std::ofstream File::Write() {
  std::ofstream fileStream;
  fileStream.open(_path.c_str(), std::ios::out | std::ios::binary);
  return fileStream;
}

File::~File() {}