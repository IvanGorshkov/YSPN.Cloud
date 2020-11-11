#pragma once

#include <string>

class PostgreSQLDB {
 public:
  PostgreSQLDB();
  virtual void CreateDB() = 0;
  virtual ~PostgreSQLDB() = default;
 protected:
  int _port;
  std::string _user;
  std::string _databaseName;
  std::string _password;
  std::string _ip;
  void *_database;
  bool dbExist();
  void connect();
  void close();
 private:
  void parseJSON();
};
