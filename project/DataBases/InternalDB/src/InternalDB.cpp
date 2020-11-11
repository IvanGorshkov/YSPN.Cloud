#include "InternalDB.h"
InternalDB::InternalDB(const std::string& databaseName): _databaseName(databaseName) {
  if (!dbExist())  {
    createDB();
  }
  if (!isConnect()) { return; }
  _userId = selectUserId();
  _deviceId = selectDeviceId();
  _syncFolder = selectFolder();
  close();
}

void InternalDB::createDB() {
  creatTable();
}

bool InternalDB::isConnect() {
  if (true) {
	if (!dbExist()) {
	  createDB();
	}
	connect();
	return true;
  }
  return true;
}

void InternalDB::creatTable() {}

int InternalDB::GetUserId() {
  if (!isConnect()) { return -1; }
  close();
  return 1;
}

int InternalDB::GetDeviceId() {
  if (!isConnect()) { return -1; }
  close();
  return 1;
}

std::string InternalDB::GetSyncFolder() {
  if (!isConnect()) { return ""; }
  close();
  return "path";
}

void InternalDB::InsertUser() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::DeleteUser() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::ExistUser() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::UpdateSyncFolder() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::UpdatePassword() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::SelectUserPassword() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::InsertFile() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::SelectFile() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::UpdateFile() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::InsertChunk() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::SelectChunk() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::UpdateChunk() {
  if (!isConnect()) { return; }
  close();
}

void InternalDB::connect() { }

void InternalDB::close() { }

bool InternalDB::dbExist() {
  return true;
}

int InternalDB::selectDeviceId() {
  if (!isConnect()) { return -1; }
  close();
  return 1;
}

int InternalDB::selectUserId() {
  if (!isConnect()) { return -1; }
  close();
  return 1;
}

std::string InternalDB::selectFolder() {
  if (!isConnect()) { return ""; }
  close();
  return "path";
}