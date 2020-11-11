#pragma once
#include <string>

class InternalDB {
 public:
  InternalDB(std::string databaseName);
  void CreateDB();
  int GetUserId();
  int GetDeviceId();
  std::string GetSyncFolder();
  void InsertUser();
  void DeleteUser();
  void ExistUser();
  void UpdateSyncFolder();
  void UpdatePassword();
  void SelectUserPassword();
  void InsertFile();
  void SelectFile();
  void UpdateFile();
  void InsertChunk();
  void SelectChunk();
  void UpdateChunk();
  bool dbExist();

 private:
  int _userId;
  int _deviceId;
  std::string _syncFolder;
  std::string  _database;
  std::string _databaseName;
  int selectDeviceId();
  int selectUserId();
  std::string selectFolder();
  void creatTable();
  bool isConnect();
  void close();
  void connect();
};
