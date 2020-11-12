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