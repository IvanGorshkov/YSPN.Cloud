#pragma once

#include "structs/FileMeta.h"
#include "structs/Chunk.h"
#include "structs/ChunkMeta.h"
#include "structs/FileChunksMeta.h"
#include "structs/FileInfo.h"
#include "InternalDB.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace bfs = boost::filesystem;

class Indexer {
 public:
  explicit Indexer(std::shared_ptr<InternalDB> internalDB);

  FileMeta GetFileMeta(const bfs::path &, bool = false, boost::optional<bfs::path> = boost::none);
  FileInfo GetFileInfo(FileMeta &, std::vector<Chunk> &);

 private:
  std::shared_ptr<InternalDB> _internalDB;
};
