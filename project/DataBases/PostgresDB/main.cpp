#include <iostream>
#include "PostgreSQLDB.h"
#include "MetaDataDB.h"

int main() {
  MetaDataDB &postgres_sqldb = MetaDataDB::shared("user=ysnp dbname=ysnpcloud");
  postgres_sqldb.Connect();
  auto file = FileMeta{.fileId = 1,
      .version = 1,
      .fileName = "test",
      .fileExtension = "txt",
      .filePath = "static/",
      .fileSize = 1,
      .chunksCount = 1,
      .isCurrent = true,
      .isDownload = true,
      .isDeleted = false,
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

  auto fileInfo =
      FileInfo{.userId = 3, .file = file, .chunkMeta = chunksMetaVector, .fileChunksMeta = fileChunksMetaVector};
  try {
    postgres_sqldb.InsertFile(fileInfo);
  } catch (PostgresExceptions exceptions) {
    std::cout << exceptions.what() << std::endl;
  }
  return 0;
}