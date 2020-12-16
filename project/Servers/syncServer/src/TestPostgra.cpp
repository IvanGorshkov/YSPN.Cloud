#include "TestPostgra.h"
#include <boost/log/trivial.hpp>

TestPostgra::TestPostgra() {
  BOOST_LOG_TRIVIAL(debug) << "TestPostgra: create config";
  auto file = FileMeta{.fileId = 1,
      .version = 1,
      .fileName = "test",
      .fileExtension = "txt",
      .filePath = "static/",
      .fileSize = 1,
      .chunksCount = 1,
      .isCurrent = true,
      .isDeleted = false,
      .updateDate = "31.12.1970",
      .createDate = "31.12.1970"};

  std::vector<ChunkMeta> chunksMetaVector;
  for (int i = 1; i < 3; ++i) {
    auto chunkMeta = ChunkMeta{.chunkId = i};
    chunksMetaVector.push_back(chunkMeta);
  }

  std::vector<FileChunksMeta> fileChunksMetaVector;
  for (int i = 1; i < 3; ++i) {
    auto fileChunkMeta = FileChunksMeta{.chunkId = i, .chunkOrder = i};
    fileChunksMetaVector.push_back(fileChunkMeta);
  }
  _fileInfo.push_back(FileInfo{.userId = 1, .file = file, .chunkMeta = chunksMetaVector, .fileChunksMeta = fileChunksMetaVector});
}

TestPostgra::~TestPostgra() {
  BOOST_LOG_TRIVIAL(debug) << "TestPostgra: delete config";
}

TestPostgra &TestPostgra::GetInstance() {
  BOOST_LOG_TRIVIAL(debug) << "TestPostgra: GetInstance";
  static TestPostgra instance;
  return instance;
}

std::vector<FileInfo> TestPostgra::GetFileInfo() {
  BOOST_LOG_TRIVIAL(debug) << "TestPostgra: GetFileInfo";
  return std::move(_fileInfo);
}

void TestPostgra::SetFileInfo(std::vector<FileInfo> fileInfo) {
  BOOST_LOG_TRIVIAL(debug) << "TestPostgra: SetFileInfo";
  _fileInfo = std::move(fileInfo);
}
