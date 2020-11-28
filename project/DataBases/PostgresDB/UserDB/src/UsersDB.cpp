#include "UsersDB.h"

UsersDB::UsersDB() {
  if (!dbExist())  {
	createDB();
  }

  connect();
}

void UsersDB::createDB() {
}

void UsersDB::Login() {
  if (!connect()) { return; }

}

void UsersDB::Registration() {
 if (!connect()) { return; }

}

void UsersDB::CheckLogin() {
  if (!connect()) { return; }
}

void UsersDB::ChangePassword() {
  if (!connect()) { return; }
}

UsersDB::~UsersDB() {
  close();
}