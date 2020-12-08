#pragma once

#include "PostgreSQLDB.h"

class MetaDataDB: public PostgresSQLDB{
 protected:
 public:
  MetaDataDB(std::string_view info);
  static MetaDataDB& shared(std::string_view info);
  ~MetaDataDB();
  void InsertFile();
  void InsertChunk();
  void InsertFileChunk();
  void SelectFile();
  void SelectChunk();
  void SelectFileChunk();
  void SelectFilesByUser();
 private:

};
