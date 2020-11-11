#pragma once

#include <string>

class MongoDB {
 public:
  MongoDB();
  ~MongoDB();
  void InsertChunk();
  void GetChunk();
 private:
  std::string _instance;
  std::string _uri;
  std::string _client;
  std::string _database;
  void parseJSON();
  void connect();
  void close();
  void createDB();
};
