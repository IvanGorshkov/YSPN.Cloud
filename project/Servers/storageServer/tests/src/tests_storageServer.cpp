#include "gtest_utils.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class StorageServerMock : public StorageServer {
 public:
  explicit StorageServerMock(MongoDB *db) : StorageServer(db) {}
  MOCK_METHOD0(runNetworkServer, void());
  MOCK_METHOD0(listeningConnection, void());
  MOCK_METHOD1(onConnect, void(UserSession *session));
  MOCK_METHOD0(uploadChunk, void());
  MOCK_METHOD0(downloadChunk, void());
};

class UserSessionMock : public UserSession {
 public:
  MOCK_METHOD0(GetRequest, void());
  MOCK_METHOD0(PutResponce, void());
};

class MongoDBMock : public MongoDB {
 public:
  MOCK_METHOD0(InsertChunk, void());
  MOCK_METHOD0(GetChunk, void());
};

TEST(StorageServer, startServer) {
  MongoDB db;
  StorageServerMock mock(&db);
  EXPECT_CALL(mock, runNetworkServer());
  EXPECT_CALL(mock, listeningConnection());
}

TEST(StorageServer, onConnect) {
  MongoDB db;
  StorageServerMock mock(&db);
  UserSession session;
  EXPECT_CALL(mock, onConnect(&session));
  mock.listeningConnection();
}

TEST(StorageServer, GetRequest) {
  MongoDB db;
  UserSessionMock mock;
  EXPECT_CALL(mock, GetRequest());
  StorageServer server(&db);
  server.onConnect(&mock);
}

TEST(StorageServer, PutResponce) {
  MongoDB db;
  UserSessionMock mock;
  EXPECT_CALL(mock, PutResponce());
  StorageServer server(&db);
  server.onConnect(&mock);
}

TEST(StorageServer, uploadChunk) {
  MongoDB db;
  StorageServerMock mock(&db);
  EXPECT_CALL(mock, uploadChunk());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, downloadChunk) {
  MongoDB db;
  StorageServerMock mock(&db);
  EXPECT_CALL(mock, downloadChunk());
  UserSession session;
  mock.onConnect(&session);
}

TEST(StorageServer, MongoInsertChunk) {
  MongoDBMock mock;
  EXPECT_CALL(mock, InsertChunk());
  StorageServer server(&mock);
  server.uploadChunk();
}

TEST(StorageServer, MongoGetChunk) {
  MongoDBMock mock;
  EXPECT_CALL(mock, GetChunk());
  StorageServer server(&mock);
  server.downloadChunk();
}