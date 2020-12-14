#pragma once

#include <string>

struct FileMeta {
  FileMeta(int fileId,
           int userId,
           int version,
           const std::string &fileName,
           const std::string &fileExtension,
           int fileSize,
           int chunksCount,
           bool isCurrent,
           const std::string &createDate,
           const std::string &updateDate)
      : fileId(fileId),
        userId(userId),
        version(version),
        fileName(fileName),
        fileExtension(fileExtension),
        fileSize(fileSize),
        chunksCount(chunksCount),
        isCurrent(isCurrent),
        createDate(createDate),
        updateDate(updateDate) {
  }

  int fileId;
  int userId;
  int version;
  std::string fileName;
  std::string filePath;
  std::string fileExtension;
  int fileSize;
  int chunksCount;
  bool isCurrent;
  std::string createDate;
  std::string updateDate;
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