#pragma once

#include <string>

struct Chunk {
  Chunk(int userId, int chunkId, const std::string &sHash, const std::string rHash, const std::string &data)
      : userId(userId),
        chunkId(chunkId),
        sHash(sHash),
        rHash(rHash),
        data(data) {
  }

  int userId;
  int chunkId;
  std::string sHash;
  std::string rHash;
  std::string data;
};

struct UserChunk {
  UserChunk(int userId, int chunkId)
      : userId(userId),
        chunkId(chunkId) {
  }

  int userId;
  int chunkId;
};