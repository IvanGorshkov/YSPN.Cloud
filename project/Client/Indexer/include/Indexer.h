#pragma once
#include "File.h"
#include "FileMeta.h"
#include "Chunk.h"
#include "ChunkMeta.h"
#include "FileChunksMeta.h"
#include "FileInfo.h"
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
  int sentUpdatetoLocalDB(void структура_для_мета_данных);
  void DeleteFile(boost::filesystem::path);
  void ModifyFile(boost::filesystem::path);
  void ChangeMetaData(boost::filesystem::path);
  int invokeChunker(boost::filesystem::path file, bool is_new_file, ChunkData data);
  int sentUpdateToQueue(void структура_для_мета_данных);
  void updateLocalDB();
  boost::property_tree::ptree createNewMetaData();
};
