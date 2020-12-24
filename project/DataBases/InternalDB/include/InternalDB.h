#pragma once

#include <sqlite3.h>
#include <string>
#include <memory>
#include <vector>
#include "User.h"
#include "InternalExceptions.h"
#include "structs/FileChunksMeta.h"
#include "structs/FileMeta.h"
#include "structs/UserDate.h"
#include "structs/FileInfo.h"
#include "structs/UserChunk.h"
#include "structs/Chunk.h"
#include "SQLiteQuery.h"

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
  void InsertUser(const User &user);
  int GetUserId() const;
  int GetDeviceId() const;
  std::string GetSyncFolder() const;
  void UpdateSyncFolder(const std::string &newFolder);

  UserDate GetLastUpdate();
  void SaveLastUpdate();

  bool IsFileExist(const int &fileId);
  int GetFileId(const std::string &path, const std::string &name, const std::string &extension);
  FileMeta GetFile(const std::string &path, const std::string &name, const std::string &extension);
  FileMeta SelectFile(const int &fileId);
  std::vector<FileMeta> SelectAllFiles();

  std::vector<UserChunk> GetUserChunks(const int &fileId);
  std::vector<FileChunksMeta> GetFileChunksMeta(const int &fileId);

  void DeleteFile(const int &fileId);
  void DownloadFile(const int &fileId);
  void InsertAndIndexFile(FileMeta &file);
  void UpdateAndIndexFile(FileMeta &file);
  void DeleteAndIndexFile(FileMeta &file);
  void RenameAndIndexFile(FileMeta &file);
  void InsertAndIndexFileChunk(FileChunksMeta &fileChunk, const int &fileId);

  void InsertOrUpdateFilesInfo(std::vector<FileInfo> &filesInfo);
  void insertOneFile(FileMeta &file);
  void updateOneFile(FileMeta &file);

  void InsertOrUpdateFileInfo(FileInfo &fileInfo);

  void DeleteUser(size_t id);
  void UpdatePassword(const std::string &newPassword);
  std::string SelectUserPassword();

  void InsertChunk(FileChunksMeta &chunks, int idFile);

  void UpdateFile(const FileMeta &file);
  void InsertFile(std::vector<FileMeta> &files);

 private:
  std::string _databaseName;
  int _userId;
  int _deviceId;
  std::string _syncFolder;
  std::string _lastUpdate;
  std::string _lastTMPUpdate;
  std::unique_ptr<sqlite3, sqlite3_deleter> _database;
  std::unique_ptr<sqlite3_stmt, sqlite3_stmt_deleter> _stmt;
  int selectDeviceId();
  int selectUserId();
  std::string selectFolder();
  int selectId(const std::string &query);
  void createTable();
  bool update(const std::string &query);
  void deleteInfo(const std::string &query);
  bool connect();
  void close();
  void insert(const std::string &query);
  std::string selectLastUpdate();
  std::string selectStr(const std::string &query);
  FileMeta getOneFile();
  void updateOneChunk(FileChunksMeta &chunk, const int id);
  bool existUser();
  static std::string getTime(std::string &time);
};
