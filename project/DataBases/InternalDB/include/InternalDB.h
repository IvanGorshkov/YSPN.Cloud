#pragma once
#include <string>
#include <sqlite3.h>
#include <memory>
#include <boost/date_time.hpp>
#include "structs/FileChunksMeta.h"
#include "User.h"
#include "structs/FileMeta.h"
#include "structs/UserDate.h"
#include "structs/FileInfo.h"
#include "structs/UserChunk.h"
#include <vector>

struct sqlite3_deleter {
  void operator()(sqlite3 *sql) {
    sqlite3_close_v2(sql);
  }
};

struct sqlite3_stmt_deleter {
  void operator()(sqlite3_stmt *sql) {
    sqlite3_finalize(sql);
  }
};

class InternalDB {
 public:
  explicit InternalDB(std::string databaseName);
  int GetUserId() const;
  int GetDeviceId() const;
  std::string GetSyncFolder() const;
  void InsertUser(const User &user);
  void DeleteUser(size_t id);
  void UpdateSyncFolder(const std::string &newFolder);
  void UpdatePassword(const std::string &newPassword);
  std::string SelectUserPassword();
  void InsertFile(const std::vector<FileMeta> &files);
  FileMeta SelectFile(size_t idFile);
  void InsertChunk(const FileChunksMeta& chunks, const int idFile) ;
  void SelectChunk();
  void UpdateFile(const FileMeta& file);
  UserDate GetLastUpdate();
  void SaveLastUpdate();
  std::vector<FileMeta> SelectAllFiles();
  void InsertFileInfo(const std::vector<FileInfo>& filesInfo);
  std::vector<UserChunk> GetUsersChunks(const int idFile);
  bool IsFileExist(const int idFile);
  void DeleteFile(const FileMeta& filesInfo);
 private:
  std::string _databaseName;
  int _userId;
  int _deviceId;
  std::string _syncFolder;
  std::string _lastUpdate;
  boost::posix_time::ptime _lastTMPUpdate;
  std::unique_ptr<sqlite3, sqlite3_deleter> _database;
  std::unique_ptr<sqlite3_stmt, sqlite3_stmt_deleter> _stmt;
  int selectDeviceId();
  int selectUserId();
  std::string selectFolder();
  int selectId(const std::string &query);
  void creatTable();
  bool update(const std::string &query);
  void deleteInfo(const std::string &query);
  virtual bool connect();
  virtual void close();
  void insert(const std::string &query);
  std::string selectLastUpdate();
  std::string selectStr(const std::string &query);
  FileMeta getOneFile();
  bool existUser();
  void insertOneFile(const FileMeta& file);
  void updateOneFile(const FileMeta& file);
  void updateOneChunk(const FileChunksMeta &chunk, const int id);
};
