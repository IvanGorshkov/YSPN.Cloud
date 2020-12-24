#pragma once

#include <vector>
#include <string>
#include "PostgreSQLDB.h"
#include "PostgresExceptions.h"
#include "structs/UserInfo.h"
#include "structs/UserIds.h"

class UsersDB: public PostgresSQLDB {
 protected:
public:
  ~UsersDB() override = default;
  static UsersDB &shared(std::string_view info);
  UserIds Login(const UserInfo& userInfo);
  UserIds Registration(const UserInfo& userInfo);
 private:
  explicit UsersDB(std::string_view info);
  UsersDB(const UsersDB &users_db);
  UsersDB &operator=(const UsersDB &users_db);
  int userExist(const std::string &query);
  int getUserId(const std::string &query, PostgresExceptions exceptions);

};
