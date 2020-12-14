#pragma once

#include <vector>
#include "structs/File.h"
#include "structs/ChunkMeta.h"
#include "structs/FileChunksMeta.h"

struct FileMeta {
  File file;
  std::vector<ChunkMeta> chunkMeta;
  std::vector<FileChunksMeta> fileChunksMeta;
};