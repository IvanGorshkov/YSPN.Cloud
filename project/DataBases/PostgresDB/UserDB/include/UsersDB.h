#pragma once

#include "PostgreSQLDB.h"

class UsersDB: public PostgreSQLDB {
public:
  UsersDB();
  ~UsersDB();
  void CreateDB();
  void Login();
  void Registration();
  void CheckLogin();
  void ChangePassword();
};
