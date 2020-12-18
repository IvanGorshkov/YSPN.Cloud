#include <iostream>
#include "MongoDB.h"

int main(int, char **) {
  Chunk chunks;
  chunks.chunkId = 1;
  chunks.rHash = "JR7udj)Hdjj@f;d";
  chunks.sHash = "JR7udjJR7udjJR7udjJR7udj";
  chunks.userId = 9;
  chunks.data = "9asdasd";
  std::vector<Chunk> hhh;
  hhh.push_back(chunks);
  auto &mongo_db = MongoDB::shared();
  mongo_db.InsertChunk(hhh);
  UserChunk chunk;
  chunk.chunkId = 1;
  chunk.userId = 9;
  std::vector<UserChunk> hhhs;
  hhhs.push_back(chunk);
  mongo_db.GetChunk(hhhs);

}
