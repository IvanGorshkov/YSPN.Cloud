#pragma once
#include "Chunk.h"
#include "File.h"
#include <openssl/md5.h>
#include <iomanip>
#include "zlib.h"
#include <memory>
#include <algorithm>
#include <iterator>
#include <array>
#include <strstream>

class Chunker {
 public:
  explicit Chunker(File);
  int SentNewChunk();
  int SentNewPosition();
  File *GetFile();
  std::vector<Chunk> ChunkFile();
  std::vector<Chunk> UpdateChunkFile(const std::vector<Chunk>&);
  void MergeFile(std::vector<Chunk>);
  void ChunkCompare(Chunk data);
 private:
  File _file;
  std::string getRHash(char*, int);
  std::string getSHash(char*, int);
  std::string getOldCheckSums();

};