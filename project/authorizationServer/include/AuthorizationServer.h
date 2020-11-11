#pragma once

class AuthorizationServer {
 public:
  explicit AuthorizationServer();

 private:
  void runNetworkServer();
  void listeningConnection();
  void onConnect();
  void checkLogin();
  void changePassword();
  void login();
  void registration();

 private:
//  UsersDB _userDB;
//  NetworkServer _networkServer;
};