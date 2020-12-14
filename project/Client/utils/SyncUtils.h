#pragma once

#include <string>

struct FileMeta {
  FileMeta(const std::string fileName,
           const std::string &fileExtension,
           int fileSize,
           const time_t &createDate,
           const time_t &updateDate)
      :
        fileName(fileName),
        fileExtension(fileExtension),
        fileSize(fileSize),
        createDate(createDate),
        updateDate(updateDate) {
  }

  int fileId;
  int userId;
  int version;
  std::string fileName;
  std::string fileExtension;
  int fileSize;
  int chunksCount;
  bool isCurrent;
  time_t createDate;
  time_t updateDate;
};

struct ChunkMeta {
  ChunkMeta(int userId, int chunkId, int chunkSize)
      : userId(userId),
        chunkId(chunkId),
        chunkSize(chunkSize) {
  }

  int userId;
  int chunkId;
  int chunkSize;
};

struct FileChunksMeta {
  FileChunksMeta(int userId, int fileId, int version, int chunkId, int chunkOrder)
      : userId(userId),
        fileId(fileId),
        version(version),
        chunkId(chunkId),
        chunkOrder(chunkOrder) {
  }

  int userId;
  int fileId;
  int version;
  int chunkId;
  int chunkOrder;
};

struct CLientMeta {
  CLientMeta(int userId, const std::string &date)
      : userId(userId),
        date(date) {
  }

  int userId;
  std::string date;
};