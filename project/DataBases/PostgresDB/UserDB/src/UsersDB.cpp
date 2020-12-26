#include "UsersDB.h"
#include <boost/lexical_cast.hpp>

UsersDB::UsersDB(std::string_view info)
    : PostgresSQLDB(info) {
}

UsersDB &UsersDB::shared(std::string_view info) {
  static UsersDB shared(info);
  return shared;
}

UserIds UsersDB::Login(const UserInfo &userInfo) {
  try {
    pqExec("begin;", PostgresExceptions("invalid to start transaction"));  // Начало транзакции
    pqExec("savepoint f_savepoint;", PostgresExceptions("invalid to insert"));  // Точка сохранения
    std::string query = "SELECT count(*) Users Where login like '" + userInfo.login + "' ;";
    userExist(query);
    query =
        "SELECT id from Users Where login like '" + userInfo.login + "' and password like '" + userInfo.password
            + "';";
    int id = getUserId(query, PostgresExceptions("Password is not the same"));
    UserIds usr{.id = id};
    pqExec("commit;", PostgresExceptions("invalid to end transation"));
    return usr;
  } catch (PostgresExceptions &exceptions) {
    throw PostgresExceptions(exceptions.what());
  }
}

UserIds UsersDB::Registration(const UserInfo &userInfo) {
  try {
    pqExec("begin;", PostgresExceptions("invalid to start transaction"));  // Начало транзакции
    pqExec("savepoint f_savepoint;", PostgresExceptions("invalid to insert"));  // Точка сохранения

    std::string query = "SELECT count(*) Users Where login like '" + userInfo.login + "' ;";
    userExist(query);

    query = "INSERT INTO Users (login, password) VALUES ('" + userInfo.login + "', '" + userInfo.password + "');";
    pqExec(query, PostgresExceptions("invalid to register user"));

    UserIds usr{.id = getUserId(query, PostgresExceptions("invalid to select id"))};
    pqExec("commit;", PostgresExceptions("invalid to end transation"));
    return usr;
  } catch (PostgresExceptions &exceptions) {
    throw PostgresExceptions(exceptions.what());
  }
}

int UsersDB::getUserId(const std::string &query, PostgresExceptions exceptions) {
  PGresult *res = PQexec(_conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    PQexec(_conn, "rollback to savepoint f_savepoint;");
    PQclear(res);
    throw exceptions;
  }
  int id = boost::lexical_cast<int>(PQgetvalue(res, 0, 0));
  PQclear(res);
  return id;
}

void UsersDB::userExist(const std::string &query) {
  PGresult *res = PQexec(_conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    PQexec(_conn, "rollback to savepoint f_savepoint;");
    PQclear(res);
    throw PostgresExceptions("faild to select count of users");
  }
  int id = boost::lexical_cast<int>(PQgetvalue(res, 0, 0));
  PQclear(res);

  if (id == 1) {
    throw PostgresExceptions("User already exist");
  }
}
