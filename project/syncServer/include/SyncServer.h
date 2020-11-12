#pragma once

class NetworkServer {
 public:
  virtual void GetRequest() {}
  virtual void PutResponce() {}
};

class MetaDataDB {
 public:
  void InsertFile() {}
  void InsertChunk() {}
  void InsertFileChunk() {}
  void SelectFile() {}
  void SelectChunk() {}
  void SelectFileChunk() {}
  void SelectFilesByUser() {}
};

class SyncServer {
 public:
  explicit SyncServer(NetworkServer *_network, MetaDataDB *_db);
  virtual void listeningConnection();
  virtual void onConnect();
  virtual void uploadFileMeta();
  virtual void downloadFileMeta();
  virtual void uploadChunkMeta();
  virtual void downloadChunkMeta();
  virtual void uploadFileChunksMeta();
  virtual void downloadFileCHunksMeta();
  virtual void findFiles();

 private:
  virtual void runNetworkServer();

 private:
  MetaDataDB *_metaDataDB;
  NetworkServer *_networkServer;
};