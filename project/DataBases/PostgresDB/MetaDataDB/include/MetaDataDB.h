#pragma once

#include "PostgreSQLDB.h"
#include "FileMeta.h"

class MetaDataDB: public PostgresSQLDB{
 protected:
 public:
  MetaDataDB(std::string_view info);
  static MetaDataDB& shared(std::string_view info);
  ~MetaDataDB();
  void InsertFile(const FileMeta& fileMeta) const;
  void InsertChunk();
  void InsertFileChunk();
  void SelectFile();
  void SelectChunk();
  void SelectFileChunkVersion();
  void SelectFilesByUser();
 private:

};
