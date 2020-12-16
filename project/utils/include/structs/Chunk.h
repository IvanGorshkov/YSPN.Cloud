#pragma once

#include <string>
#include <vector>

struct Chunk {
  int userId;
  int chunkId;
  int chunkSize;
  std::string sHash;
  std::string rHash;
  std::vector<char> data;
};