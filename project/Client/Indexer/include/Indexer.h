#pragma once

#include "structs/FileMeta.h"
#include "structs/Chunk.h"
#include "structs/ChunkMeta.h"
#include "structs/FileChunksMeta.h"
#include "structs/FileInfo.h"
#include "InternalDB.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace bfs = boost::filesystem;

class Indexer {
 public:
  Indexer(std::shared_ptr<InternalDB> internalDB);
  FileMeta GetFileMeta(boost::filesystem::path, bool);
  FileInfo GetFileInfo(const FileMeta&, std::vector<Chunk>&);

 private:
  std::shared_ptr<InternalDB> _internalDB;
};
