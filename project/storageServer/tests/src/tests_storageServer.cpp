#include "gtest_utils.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class StorageServerMock : public StorageServer {
 public:
  explicit StorageServerMock(NetworkServer *server, MongoDB *db) : StorageServer(server, db) {}
  MOCK_METHOD0(runNetworkServer, void());
  MOCK_METHOD0(listeningConnection, void());
  MOCK_METHOD0(onConnect, void());
  MOCK_METHOD0(uploadChunk, void());
  MOCK_METHOD0(downloadChunk, void());
};

class NetworkServerMock : public NetworkServer {
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
  NetworkServer network;
  MongoDB db;
  StorageServerMock mock(&network, &db);
  EXPECT_CALL(mock, runNetworkServer());
  EXPECT_CALL(mock, listeningConnection());
}

TEST(StorageServer, onConnect) {
  NetworkServer network;
  MongoDB db;
  StorageServerMock mock(&network, &db);
  EXPECT_CALL(mock, onConnect());
  mock.listeningConnection();
}

TEST(StorageServer, GetRequest) {
  NetworkServerMock mock;
  MongoDB db;
  EXPECT_CALL(mock, GetRequest());
  StorageServer server(&mock, &db);
  server.onConnect();
}

TEST(StorageServer, PutResponce) {
  NetworkServerMock mock;
  MongoDB db;
  EXPECT_CALL(mock, PutResponce());
  StorageServer server(&mock, &db);
  server.onConnect();
}

TEST(StorageServer, uploadChunk) {
  NetworkServer network;
  MongoDB db;
  StorageServerMock mock(&network, &db);
  EXPECT_CALL(mock, uploadChunk());
  mock.onConnect();
}

TEST(StorageServer, downloadChunk) {
  NetworkServer network;
  MongoDB db;
  StorageServerMock mock(&network, &db);
  EXPECT_CALL(mock, downloadChunk());
  mock.onConnect();
}

TEST(StorageServer, MongoInsertChunk) {
  NetworkServer network;
  MongoDBMock mock;
  EXPECT_CALL(mock, InsertChunk());
  StorageServer server(&network, &mock);
  server.uploadChunk();
}

TEST(StorageServer, MongoGetChunk) {
  NetworkServer network;
  MongoDBMock mock;
  EXPECT_CALL(mock, GetChunk());
  StorageServer server(&network, &mock);
  server.downloadChunk();
}