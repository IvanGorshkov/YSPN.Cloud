#pragma once
#include <string>

struct Chunks {
  int id;
  int idFile;
  int chunkSize;
  std::string rapidHash;
  std::string staticHash;
};