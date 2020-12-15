//
// Created by Ivan Gorshkov on 23.11.2020.
//
#include <iostream>
#include "InternalDB.h"
#include <vector>

int main() {
  InternalDB myDB("myDB.sqlite");
  auto file = FileMeta{.fileId = 1,
	  .version = 1,
	  .fileName = "test",
	  .fileExtension = "txt",
	  .filePath = "static/",
	  .fileSize = 1,
	  .chunksCount = 1,
	  .isCurrent = true,
	  .isDownload = true,
	  .updateDate = "31.12.1970",
	  .createDate = "31.12.1970"};

  std::vector<ChunkMeta> chunksMetaVector;
  for (int i = 0; i < 2; ++i) {
	auto chunkMeta = ChunkMeta{.chunkId = i};
	chunksMetaVector.push_back(chunkMeta);
  }

  std::vector<FileChunksMeta> fileChunksMetaVector;
  for (int i = 0; i < 2; ++i) {
	auto fileChunkMeta = FileChunksMeta{.chunkId = i, .chunkOrder = i};
	fileChunksMetaVector.push_back(fileChunkMeta);
  }

  auto fileInfo = FileInfo{.file = file, .chunkMeta = chunksMetaVector, .fileChunksMeta = fileChunksMetaVector};
  std::vector<FileInfo> f;
  f.push_back(fileInfo);
  myDB.InsertFileMeta(f);

  return 0;
}