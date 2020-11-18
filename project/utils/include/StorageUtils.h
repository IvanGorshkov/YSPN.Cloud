#pragma once





struct UserChunk {
  UserChunk(int userId, int chunkId)
      : userId(userId),
        chunkId(chunkId) {
  }

  int userId;
  int chunkId;
};