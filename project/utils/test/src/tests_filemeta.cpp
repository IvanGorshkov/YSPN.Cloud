#include "gtest_utils.h"

TEST(FileMeta, Struct) {
  auto file = FileMeta{.fileId = 1,
      .version = 1,
      .fileName = "test",
      .fileExtension = "txt",
      .filePath = "static/",
      .fileSize = 1,
      .chunksCount = 1,
      .isCurrent = true,
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

  auto fileMeta = FileInfo{.file = file, .chunkMeta = chunksMetaVector, .fileChunksMeta = fileChunksMetaVector};

  std::vector<FileInfo> vec;
  vec.push_back(fileMeta);

  auto serializer = SerializerFileInfo(1, 1, vec);

  std::stringstream ss;
  pt::write_json(ss, serializer.GetJson());
  std::cout << ss.str() << std::endl;
}
