#include "gtest_utils.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class SyncServerMock : public SyncServer {
 public:
  explicit SyncServerMock(NetworkServer *server, MetaDataDB *db) : SyncServer(server, db) {}
  MOCK_METHOD0(runNetworkServer, void());
  MOCK_METHOD0(onConnect, void());
  MOCK_METHOD0(listeningConnection, void());
  MOCK_METHOD0(uploadFileMeta, void());
  MOCK_METHOD0(downloadFileMeta, void());
  MOCK_METHOD0(uploadChunkMeta, void());
  MOCK_METHOD0(downloadChunkMeta, void());
  MOCK_METHOD0(uploadFileChunksMeta, void());
  MOCK_METHOD0(downloadFileCHunksMeta, void());
  MOCK_METHOD0(findFiles, void());
};

class NetworkServerMock : public NetworkServer {
 public:
  MOCK_METHOD0(GetRequest, void());
  MOCK_METHOD0(PutResponce, void());
};

class MetaDataDBMock : public MetaDataDB {
 public:
  MOCK_METHOD0(InsertFile, void());
  MOCK_METHOD0(InsertChunk, void());
  MOCK_METHOD0(InsertFileChunk, void());
  MOCK_METHOD0(SelectFile, void());
  MOCK_METHOD0(SelectChunk, void());
  MOCK_METHOD0(SelectFileChunk, void());
  MOCK_METHOD0(SelectFilesByUser, void());
};

TEST(StorageServer, startServer) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, runNetworkServer());
  EXPECT_CALL(mock, listeningConnection());
}

TEST(StorageServer, onConnect) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, onConnect());
  mock.listeningConnection();
}

TEST(StorageServer, GetRequest) {
  NetworkServerMock mock;
  MetaDataDB db;
  EXPECT_CALL(mock, GetRequest());
  SyncServer server(&mock, &db);
  server.onConnect();
}

TEST(StorageServer, PutResponce) {
  NetworkServerMock mock;
  MetaDataDB db;
  EXPECT_CALL(mock, PutResponce());
  SyncServer server(&mock, &db);
  server.onConnect();
}

TEST(StorageServer, uploadFileMeta) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, uploadFileMeta());
  mock.onConnect();
}

TEST(StorageServer, downloadFileMeta) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, downloadFileMeta());
  mock.onConnect();
}

TEST(StorageServer, uploadChunkMeta) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, uploadChunkMeta());
  mock.onConnect();
}

TEST(StorageServer, downloadChunkMeta) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, downloadChunkMeta());
  mock.onConnect();
}

TEST(StorageServer, uploadFileChunksMeta) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, uploadFileChunksMeta());
  mock.onConnect();
}

TEST(StorageServer, downloadFileCHunksMeta) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, downloadFileCHunksMeta());
  mock.onConnect();
}

TEST(StorageServer, findFiles) {
  NetworkServer network;
  MetaDataDB db;
  SyncServerMock mock(&network, &db);
  EXPECT_CALL(mock, findFiles());
  mock.onConnect();
}

TEST(StorageServer, MetadataInsertFile) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, InsertFile());
  server.uploadFileMeta();
}

TEST(StorageServer, MetadataInsertChunk) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, InsertChunk());
  server.uploadChunkMeta();
}

TEST(StorageServer, MetadataInsertFileChunk) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, InsertFileChunk());
  server.uploadFileChunksMeta();
}

TEST(StorageServer, MetadataSelectFile) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, SelectFile());
  server.downloadFileMeta();
}

TEST(StorageServer, MetadataSelectChunk) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, SelectChunk());
  server.downloadChunkMeta();
}

TEST(StorageServer, MetadataSelectFileChunk) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, SelectFileChunk());
  server.downloadFileCHunksMeta();
}

TEST(StorageServer, MetadataSelectFilesByUser) {
  NetworkServer network;
  MetaDataDBMock mock;
  SyncServer server(&network, &mock);
  EXPECT_CALL(mock, SelectFilesByUser());
  server.findFiles();
}