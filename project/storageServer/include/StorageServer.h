#pragma once

class UserSession {
 public:
  virtual void GetRequest() {}
  virtual void PutResponce() {}
};

class NetworkServer {
  UserSession session;
};

class MongoDB {
 public:
  void InsertChunk() {}
  void GetChunk() {}
};

class StorageServer {
 public:
  explicit StorageServer(MongoDB *_db);
  virtual void listeningConnection();
  virtual void onConnect(UserSession *session);
  virtual void uploadChunk();
  virtual void downloadChunk();

 private:
  virtual void runNetworkServer();

 private:
  MongoDB *_mongoDB;
  NetworkServer *_networkServer;
};