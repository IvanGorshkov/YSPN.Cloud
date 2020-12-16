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
  int SentNewChunk();
  int SentNewPosition();
  File *GetFile();
  std::vector<Chunk> ChunkFile();
  std::vector<Chunk> UpdateChunkFile(const std::vector<Chunk>&);
  void MergeFile(std::vector<Chunk>);
  void ChunkCompare(Chunk data);
 private:
  File _file;
  void getRHash(Chunk &);
  void getSHash(Chunk &);
  std::string getOldCheckSums();

};