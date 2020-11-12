#pragma once

class NetworkServer {
 public:
  virtual void GetRequest() {}
  virtual void PutResponce() {}
};

class MongoDB {
 public:
  void InsertChunk() {}
  void GetChunk() {}
};

class StorageServer {
 public:
  explicit StorageServer(NetworkServer *server, MongoDB *_db);
  virtual void listeningConnection();
  virtual void onConnect();
  virtual void uploadChunk();
  virtual void downloadChunk();

 private:
  virtual void runNetworkServer();

 private:
  MongoDB *_mongoDB;
  NetworkServer *_networkServer;
};