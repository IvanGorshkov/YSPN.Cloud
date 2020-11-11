#pragma once

class SyncServer {
 public:
  SyncServer();

 private:
  void runNetworkServer();
  void listeningConnection();
  void onConnect();
  void uploadFileMeta();
  void downloadFileMeta();
  void uploadChunkMeta();
  void downloadChunk();
  void uploadFileChunksMeta();
  void downloadFileCHunksMeta();
  void findFiles();

 private:
//  MetaDataDB _metaDataDB;
//  NetworkServer: _networkServer;
};