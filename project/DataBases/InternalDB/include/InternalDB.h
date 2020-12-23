#pragma once
#include <string>
#include <sqlite3.h>
#include <memory>
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

  std::string GetSyncFolder() const;
  void UpdateSyncFolder(const std::string &newFolder);

  FileMeta SelectFile(const int &fileId);

  UserDate GetLastUpdate();
  void SaveLastUpdate();

  std::vector<FileMeta> SelectAllFiles();

  void InsertOrUpdateFileInfo(FileInfo &fileInfo);

  std::vector<UserChunk> GetFileChunks(const int &fileId);

  bool IsFileExist(const int &fileId);
  void DeleteFile(const int &fileId);
  void DownloadFile(const int &fileId);
  int FindIdFile(const std::string &path, const std::string &name, const std::string &extension);

  int GetDeviceId() const;
  void InsertUser(const User &user);
  void DeleteUser(size_t id);
  void UpdatePassword(const std::string &newPassword);
  std::string SelectUserPassword();

  void SelectChunk();
  void InsertChunk(FileChunksMeta &chunks, int idFile);

  void UpdateFile(const FileMeta &file);
  void InsertFile(std::vector<FileMeta> &files);
  void InsertOrUpdateFilesInfo(std::vector<FileInfo> &filesInfo);

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
  void insertOneFile(FileMeta &file);
  void updateOneFile(FileMeta &file);
  void updateOneChunk(FileChunksMeta &chunk, const int id);
  std::string getTime(std::string &time);
};
