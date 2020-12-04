#pragma once
#include <string>
#include <sqlite3.h>
#include <memory>
#include "Chunks.h"
#include "User.h"
#include "Files.h"

struct sqlite3_deleter {
  void operator()(sqlite3* sql) {
	sqlite3_close_v2(sql);
  }
};

struct sqlite3_stmt_deleter {
  void operator()(sqlite3_stmt* sql) {
	sqlite3_finalize(sql);
  }
};

class InternalDB {
 public:
  explicit InternalDB(const std::string& databaseName);
  int GetUserId() const;
  int GetDeviceId() const;
  std::string GetSyncFolder() const;
  void InsertUser();
  void DeleteUser(size_t id);
  bool ExistUser();
  void UpdateSyncFolder(const std::string& newFolder);
  void UpdatePassword(const std::string& newPassword);
  std::string SelectUserPassword();
  void InsertFile(const Files& file);
  Files SelectFile(size_t idFile);
  void UpdateFile();
  void InsertChunk(const Chunks& chunks);
  void SelectChunk();
  void UpdateChunk();

 private:
  int _userId;
  int _deviceId;
  std::string _syncFolder;
  std::unique_ptr<sqlite3, sqlite3_deleter> _database;
  std::unique_ptr<sqlite3_stmt, sqlite3_stmt_deleter> _stmt;
  std::string _databaseName;
  int selectDeviceId();
  int selectUserId();
  std::string selectFolder();
  int selectId(const std::string& query);
  void creatTable();
  bool update(const std::string& query);
  virtual bool connect();
  virtual void close();
  static int callbackFile(void* data, int argc, char** argv, char** azColName);
  void insert(const std::string& query);
};
