#pragma once

class UserSession {
 public:
  virtual void GetRequest() {}
  virtual void PutResponce() {}
};

class NetworkServer {
  UserSession session;
};

class UsersDB {
 public:
  void Login() {}
  void Registration() {}
  void CheckLogin() {}
  void ChangePassword() {}
};

class AuthorizationServer {
 public:
  explicit AuthorizationServer(UsersDB *_db);
  virtual void listeningConnection();
  virtual void onConnect(UserSession *session);
  virtual void checkLogin();
  virtual void changePassword();
  virtual void login();
  virtual void registration();

 private:
  virtual void runNetworkServer();

 private:
  UsersDB *_userDB;
  NetworkServer *_networkServer;
};