#pragma once
#include <string>
#include <sqlite3.h>
#include <memory>
#include <boost/date_time.hpp>
#include "Chunks.h"
#include "User.h"
#include "Files.h"
#include "structs/UserDate.h"

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
  void InsertFile(const std::vector<Files> &files);
  Files SelectFile(size_t idFile);
  void UpdateFile();
  void InsertChunk(const Chunks &chunks);
  void SelectChunk();
  void UpdateChunk();
  UserDate GetLastUpdate();
  void SaveLastUpdate();
  std::vector<Files> SelectAllFiles();
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
  virtual bool connect();
  virtual void close();
  void insert(const std::string &query);
  std::string selectLastUpdate();
  std::string selectStr(const std::string &query);
  Files getOneFile();
  bool existUser();
};
