#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "structs/FileInfo.h"

class TestPostgra {
 public:
  static TestPostgra &GetInstance();
  std::vector<FileInfo> GetFileInfo();
  void SetFileInfo(std::vector<FileInfo> fileInfo);

 private:
  TestPostgra();
  ~TestPostgra();
  TestPostgra(const TestPostgra &) = delete;
  TestPostgra &operator=(const TestPostgra &) = delete;

 private:
  std::vector<FileInfo> _fileInfo;
};
