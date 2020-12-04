#pragma once

#include <string>

class MongoDB {
 public:
  MongoDB();
  ~MongoDB();
  void InsertChunk();
  void GetChunk();
 private:
  //mongocxx::instance _instance{};
  //mongocxx::database _database;
  //mongocxx::uri _uri;
  //mongocxx::client _client;
  virtual void parseJSON();
  void connect();
  void close();
  void createDB();
};
