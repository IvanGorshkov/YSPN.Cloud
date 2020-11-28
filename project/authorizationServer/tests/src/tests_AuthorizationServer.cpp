#include "gtest_utils.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class AuthorizationServerMock : public AuthorizationServer {
 public:
  explicit AuthorizationServerMock(UsersDB *db) : AuthorizationServer(db) {}
  MOCK_METHOD0(runNetworkServer, void());
  MOCK_METHOD1(onConnect, void(UserSession *session));
  MOCK_METHOD0(listeningConnection, void());
  MOCK_METHOD0(checkLogin, void());
  MOCK_METHOD0(changePassword, void());
  MOCK_METHOD0(login, void());
  MOCK_METHOD0(registration, void());
};

class UserSessionMock : public UserSession {
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
  UsersDB db;
  AuthorizationServerMock mock(&db);
  EXPECT_CALL(mock, runNetworkServer());
  EXPECT_CALL(mock, listeningConnection());
}

TEST(AuthorizationServer, onConnect) {
  UsersDB db;
  AuthorizationServerMock mock(&db);
  UserSession session;
  EXPECT_CALL(mock, onConnect(&session));
  mock.listeningConnection();
}

TEST(AuthorizationServer, GetRequest) {
  UserSessionMock mock;
  UsersDB db;
  EXPECT_CALL(mock, GetRequest());
  AuthorizationServer server(&db);
  server.onConnect(&mock);
}

TEST(AuthorizationServer, PutResponce) {
  UserSessionMock mock;
  UsersDB db;
  EXPECT_CALL(mock, GetRequest());
  AuthorizationServer server(&db);
  server.onConnect(&mock);
}

TEST(AuthorizationServer, checkLogin) {
  UsersDB db;
  AuthorizationServerMock mock(&db);
  EXPECT_CALL(mock, checkLogin());
  UserSession session;
  mock.onConnect(&session);
}

TEST(AuthorizationServer, changePassword) {
  UsersDB db;
  AuthorizationServerMock mock(&db);
  EXPECT_CALL(mock, changePassword());
  UserSession session;
  mock.onConnect(&session);
}

TEST(AuthorizationServer, login) {
  UsersDB db;
  AuthorizationServerMock mock(&db);
  EXPECT_CALL(mock, login());
  UserSession session;
  mock.onConnect(&session);
}

TEST(AuthorizationServer, registration) {
  UsersDB db;
  AuthorizationServerMock mock(&db);
  EXPECT_CALL(mock, registration());
  UserSession session;
  mock.onConnect(&session);
}

TEST(AuthorizationServer, UserCheckLogin) {
  UsersDBMock mock;
  AuthorizationServer server(&mock);
  EXPECT_CALL(mock, CheckLogin());
  server.checkLogin();
}

TEST(AuthorizationServer, UsersChangePassword) {
  UsersDBMock mock;
  AuthorizationServer server(&mock);
  EXPECT_CALL(mock, ChangePassword());
  server.changePassword();
}

TEST(AuthorizationServer, UsersLogin) {
  UsersDBMock mock;
  AuthorizationServer server(&mock);
  EXPECT_CALL(mock, Login());
  server.login();
}

TEST(AuthorizationServer, USersRegistration) {
  UsersDBMock mock;
  AuthorizationServer server(&mock);
  EXPECT_CALL(mock, Registration());
  server.registration();
}