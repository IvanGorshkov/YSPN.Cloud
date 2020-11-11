#include "UsersDB.h"

void UsersDB::CreateDB() {
  if (!dbExist())  {
	CreateDB();
  }

  connect();
}

void UsersDB::Login() {

}

void UsersDB::Registration() {

}

void UsersDB::CheckLogin() {

}

void UsersDB::ChangePassword() {

}

UsersDB::~UsersDB() {
  close();
}