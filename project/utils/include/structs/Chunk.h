#pragma once

#include <string>

struct Chunk {
  int userId;
  int chunkId;
  int chunkSize;
  std::string sHash;
  std::string rHash;
  std::string data;
};