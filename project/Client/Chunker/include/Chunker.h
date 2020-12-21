#pragma once

#define CHUNK_SIZE 4096

#include "structs/Chunk.h"
#include "File.h"
#include <openssl/md5.h>
#include <iomanip>
#include "zlib.h"
#include <memory>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <array>


class Chunker {
 public:
  explicit Chunker(const File&);

  std::vector<Chunk> ChunkFile();
  std::vector<Chunk> UpdateChunkFile(const std::vector<Chunk>&);
  void MergeFile(const std::vector<Chunk>&);
 private:
  File _file;
  static std::string getRHash(char*, int);
  static std::string getSHash(char*, int);
  Chunk createChunk(std::vector<char>, int);
};