#include "gtest_utils.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class SyncServerMock : public SyncServer {
 public:
  explicit SyncServerMock(MetaDataDB *db) : SyncServer(db) {}
  MOCK_METHOD0(runNetworkServer, void());
  MOCK_METHOD1(onConnect, void(UserSession *session));
  MOCK_METHOD0(listeningConnection, void());
  MOCK_METHOD0(uploadFileMeta, void());
  MOCK_METHOD0(downloadFileMeta, void());
  MOCK_METHOD0(uploadChunkMeta, void());
  MOCK_METHOD0(downloadChunkMeta, void());
  MOCK_METHOD0(uploadFileChunksMeta, void());
  MOCK_METHOD0(downloadFileCHunksMeta, void());
  MOCK_METHOD0(findFiles, void());
};

class UserSessionMock : public UserSession {
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
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, runNetworkServer());
  EXPECT_CALL(mock, listeningConnection());
}

TEST(StorageServer, onConnect) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  UserSession session;
  EXPECT_CALL(mock, onConnect(&session));
  mock.listeningConnection();
}

TEST(StorageServer, GetRequest) {
  MetaDataDB db;
  UserSessionMock mock;
  EXPECT_CALL(mock, GetRequest());
  SyncServer server(&db);
  server.onConnect(&mock);
}

TEST(StorageServer, PutResponce) {
  MetaDataDB db;
  UserSessionMock mock;
  EXPECT_CALL(mock, PutResponce());
  SyncServer server(&db);
  server.onConnect(&mock);
}

TEST(StorageServer, uploadFileMeta) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, uploadFileMeta());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, downloadFileMeta) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, downloadFileMeta());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, uploadChunkMeta) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, uploadChunkMeta());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, downloadChunkMeta) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, downloadChunkMeta());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, uploadFileChunksMeta) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, uploadFileChunksMeta());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, downloadFileCHunksMeta) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, downloadFileCHunksMeta());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, findFiles) {
  MetaDataDB db;
  SyncServerMock mock(&db);
  EXPECT_CALL(mock, findFiles());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, MetadataInsertFile) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, InsertFile());
  server.uploadFileMeta();
}

TEST(StorageServer, MetadataInsertChunk) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, InsertChunk());
  server.uploadChunkMeta();
}

TEST(StorageServer, MetadataInsertFileChunk) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, InsertFileChunk());
  server.uploadFileChunksMeta();
}

TEST(StorageServer, MetadataSelectFile) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, SelectFile());
  server.downloadFileMeta();
}

TEST(StorageServer, MetadataSelectChunk) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, SelectChunk());
  server.downloadChunkMeta();
}

TEST(StorageServer, MetadataSelectFileChunk) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, SelectFileChunk());
  server.downloadFileCHunksMeta();
}

TEST(StorageServer, MetadataSelectFilesByUser) {
  MetaDataDBMock mock;
  SyncServer server(&mock);
  EXPECT_CALL(mock, SelectFilesByUser());
  server.findFiles();
}