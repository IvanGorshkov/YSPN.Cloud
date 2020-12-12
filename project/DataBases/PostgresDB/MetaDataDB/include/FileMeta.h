#pragma once

#include <string>
#include <utility>

struct FileMeta {
  FileMeta()= default;
  FileMeta(int fileId,
		   int userId,
		   int version,
		   std::string fileName,
		   std::string fileExtension,
		   int fileSize,
		   int chunksCount,
		   bool isCurrent,
		   std::string createDate,
		   std::string updateDate)
	  : fileId(fileId),
		userId(userId),
		version(version),
		fileName(std::move(fileName)),
		fileExtension(std::move(fileExtension)),
		fileSize(fileSize),
		chunksCount(chunksCount),
		isCurrent(isCurrent),
		createDate(std::move(createDate)),
		updateDate(std::move(updateDate)) {
  }

  int fileId{};
  int userId{};
  int version{};
  std::string fileName;
  std::string fileExtension;
  int fileSize{};
  int chunksCount{};
  bool isCurrent{};
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