#pragma once

#include <string>

class PostgreSQLDB {
 public:
  PostgreSQLDB();
  virtual ~PostgreSQLDB() = default;
 protected:
  int _port;
  virtual void createDB() = 0;
  std::string _user;
  std::string _databaseName;
  std::string _password;
  std::string _ip;
  void *_database;
  bool dbExist();
  virtual bool connect();
  void close();
 private:
  void parseJSON();
};
