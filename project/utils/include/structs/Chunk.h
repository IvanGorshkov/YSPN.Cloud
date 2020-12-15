#pragma once

#include <string>
#include <array>

#define CHUNK_SIZE       4096

struct Chunk {
  int userId;
  int chunkId;
  int chunkSize;
  std::string sHash;
  std::string rHash;
  std::array<char, CHUNK_SIZE> data;
};