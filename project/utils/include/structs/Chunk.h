#pragma once

#include <string>
#include <vector>

#define CHUNK_SIZE       4096

struct Chunk {
  int userId;
  int chunkId;
  int chunkSize;
  std::string sHash;
  std::string rHash;
  std::vector<char> data;
};