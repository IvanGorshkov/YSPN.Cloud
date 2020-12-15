#pragma once
#include "Notification.h"
#include "File.h"
#include "SyncUtils.h"
#include "ChunkData.h"
#include "stdlib.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class Indexer {
 public:

  void GetWatcherEvent(Notification notification);
  void CreateNewFile(boost::filesystem::path);
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
