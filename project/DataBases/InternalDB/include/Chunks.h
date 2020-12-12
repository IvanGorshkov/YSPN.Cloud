#pragma once
#include <string>

struct Chunks {
  size_t id;
  size_t idFile;
  size_t chunkSize;
  std::string rapidHash;
  std::string staticHash;
};