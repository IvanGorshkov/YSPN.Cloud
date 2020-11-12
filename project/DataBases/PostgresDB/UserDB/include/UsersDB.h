#pragma once

#include "PostgreSQLDB.h"

class UsersDB: public PostgreSQLDB {
 protected:
  void createDB() override;
public:
  UsersDB();
  ~UsersDB();
  void Login();
  void Registration();
  void CheckLogin();
  void ChangePassword();
};
