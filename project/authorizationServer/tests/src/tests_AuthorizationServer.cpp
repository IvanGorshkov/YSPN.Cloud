#include "gtest_utils.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class AuthorizationServerMock : public AuthorizationServer {
 public:
  explicit AuthorizationServerMock(NetworkServer *server, UsersDB *db) : AuthorizationServer(server, db) {}
  MOCK_METHOD0(runNetworkServer, void());
  MOCK_METHOD0(onConnect, void());
  MOCK_METHOD0(listeningConnection, void());
  MOCK_METHOD0(checkLogin, void());
  MOCK_METHOD0(changePassword, void());
  MOCK_METHOD0(login, void());
  MOCK_METHOD0(registration, void());
};

class NetworkServerMock : public NetworkServer {
 public:
  MOCK_METHOD0(GetRequest, void());
  MOCK_METHOD0(PutResponce, void());
};

class UsersDBMock : public UsersDB {
 public:
  MOCK_METHOD0(Login, void());
  MOCK_METHOD0(Registration, void());
  MOCK_METHOD0(CheckLogin, void());
  MOCK_METHOD0(ChangePassword, void());
};

TEST(AuthorizationServer, startServer) {
  NetworkServer network;
  UsersDB db;
  AuthorizationServerMock mock(&network, &db);
  EXPECT_CALL(mock, runNetworkServer());
  EXPECT_CALL(mock, listeningConnection());
}

TEST(AuthorizationServer, onConnect) {
  NetworkServer network;
  UsersDB db;
  AuthorizationServerMock mock(&network, &db);
  EXPECT_CALL(mock, onConnect());
  mock.listeningConnection();
}

TEST(AuthorizationServer, GetRequest) {
  NetworkServerMock mock;
  UsersDB db;
  EXPECT_CALL(mock, GetRequest());
  AuthorizationServer server(&mock, &db);
  server.onConnect();
}

TEST(AuthorizationServer, PutResponce) {
  NetworkServerMock mock;
  UsersDB db;
  EXPECT_CALL(mock, GetRequest());
  AuthorizationServer server(&mock, &db);
  server.onConnect();
}

TEST(AuthorizationServer, checkLogin) {
  NetworkServer network;
  UsersDB db;
  AuthorizationServerMock mock(&network, &db);
  EXPECT_CALL(mock, checkLogin());
  mock.onConnect();
}

TEST(AuthorizationServer, changePassword) {
  NetworkServer network;
  UsersDB db;
  AuthorizationServerMock mock(&network, &db);
  EXPECT_CALL(mock, changePassword());
  mock.onConnect();
}

TEST(AuthorizationServer, login) {
  NetworkServer network;
  UsersDB db;
  AuthorizationServerMock mock(&network, &db);
  EXPECT_CALL(mock, login());
  mock.onConnect();
}

TEST(AuthorizationServer, registration) {
  NetworkServer network;
  UsersDB db;
  AuthorizationServerMock mock(&network, &db);
  EXPECT_CALL(mock, registration());
  mock.onConnect();
}

TEST(AuthorizationServer, UserCheckLogin) {
  NetworkServer network;
  UsersDBMock mock;
  AuthorizationServer server(&network, &mock);
  EXPECT_CALL(mock, CheckLogin());
  server.checkLogin();
}

TEST(AuthorizationServer, UsersChangePassword) {
  NetworkServer network;
  UsersDBMock mock;
  AuthorizationServer server(&network, &mock);
  EXPECT_CALL(mock, ChangePassword());
  server.changePassword();
}

TEST(AuthorizationServer, UsersLogin) {
  NetworkServer network;
  UsersDBMock mock;
  AuthorizationServer server(&network, &mock);
  EXPECT_CALL(mock, Login());
  server.login();
}

TEST(AuthorizationServer, USersRegistration) {
  NetworkServer network;
  UsersDBMock mock;
  AuthorizationServer server(&network, &mock);
  EXPECT_CALL(mock, Registration());
  server.registration();
}