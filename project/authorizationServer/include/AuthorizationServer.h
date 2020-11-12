#pragma once

class NetworkServer {
 public:
  virtual void GetRequest() {}
  virtual void PutResponce() {}
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
  explicit AuthorizationServer(NetworkServer *_network, UsersDB *_db);
  virtual void listeningConnection();
  virtual void onConnect();
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