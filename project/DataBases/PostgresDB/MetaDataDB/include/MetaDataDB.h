#pragma once

#include "PostgreSQLDB.h"
#include "structs/FileInfo.h"

class MetaDataDB: public PostgresSQLDB{
 protected:
 public:
  MetaDataDB(std::string_view info);
  static MetaDataDB& shared(std::string_view info);
  ~MetaDataDB();
  void InsertFile(const FileInfo& fileMeta);
  void InsertChunk();
  void InsertFileChunk();
  void SelectFile();
  void SelectChunk();
  void SelectFileChunkVersion();
  void SelectFilesByUser();
 private:
  int getLastIdOfFileUser(const std::string &query, PostgresExceptions exceptions);

};
