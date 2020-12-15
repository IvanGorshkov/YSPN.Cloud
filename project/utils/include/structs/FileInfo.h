#pragma once

#include <vector>
#include "structs/FileMeta.h"
#include "structs/ChunkMeta.h"
#include "structs/FileChunksMeta.h"

struct FileInfo {
  int userId;
  FileMeta file;
  std::vector<ChunkMeta> chunkMeta;
  std::vector<FileChunksMeta> fileChunksMeta;
};