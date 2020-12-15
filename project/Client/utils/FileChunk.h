#pragma once
#include "stdlib.h"
#include <array>
#include <string>

#define CHUNK_SIZE       4096

struct FileChunk {
  std::array<char, CHUNK_SIZE> data;
  size_t chunkSize;
  std::string RHash;
  std::string SHash;
  int chunk_id;
  int user_id;
};
