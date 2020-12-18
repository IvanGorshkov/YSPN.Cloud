#pragma once

#include "PostgreSQLDB.h"
#include "structs/FileInfo.h"
#include "structs/UserDate.h"

class MetaDataDB : public PostgresSQLDB {
 protected:
 public:
  static MetaDataDB &shared(std::string_view info);
  void InsertFile(const FileInfo &fileMeta);
  void InsertChunk();
  void InsertFileChunk();
  void SelectFile();
  void SelectChunk();
  void SelectFileChunkVersion();
  void SelectFilesByUser();
  std::vector<FileInfo> GetUserFilesByTime(const UserDate& userDate);
 private:
  explicit MetaDataDB(std::string_view info);
  ~MetaDataDB() override = default;
  MetaDataDB(const MetaDataDB &mongo_db);
  MetaDataDB &operator=(const MetaDataDB &mongo_db);

  int getLastIdOfFileUser(const std::string &query, PostgresExceptions exceptions);

};
