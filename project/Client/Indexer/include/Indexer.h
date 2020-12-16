#pragma once
#include "File.h"
#include "FileMeta.h"
#include "Chunk.h"
#include "ChunkMeta.h"
#include "FileChunksMeta.h"
#include "FileInfo.h"
#include "InternalDB.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace bfs = boost::filesystem;

class Indexer {
 public:
  FileMeta GetFileMeta(boost::filesystem::path, bool);
  FileInfo GetFileInfo(const FileMeta&, std::vector<Chunk>);
 private:
  InternalDB _internalDB;
};
