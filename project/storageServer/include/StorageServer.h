#pragma once

class StorageServer {
 public:
  StorageServer();

 private:
  void runNetworkServer();
  void listeningConnection();
  void onConnect();
  void uploadChunk();
  void downloadChunk();

 private:
//  MongoDB _mongoDB;
//  NetworkSever _networkServer;
};