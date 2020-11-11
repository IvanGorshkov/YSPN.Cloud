#include "InternalDB.h"
InternalDB::InternalDB(const std::string& databaseName): _databaseName(databaseName) {
  if (!dbExist())  {
    createDB();
  }
  if (!connect()) { return; }
  _userId = selectUserId();
  _deviceId = selectDeviceId();
  _syncFolder = selectFolder();
  close();
}

void InternalDB::createDB() {
  creatTable();
}

bool InternalDB::connect() {
  return true;
}

void InternalDB::creatTable() {}

int InternalDB::GetUserId() {
  if (connect()) { return -1; }
  close();
  return 1;
}

int InternalDB::GetDeviceId() {
  if (connect()) { return -1; }
  close();
  return 1;
}

std::string InternalDB::GetSyncFolder() {
  if (connect()) { return ""; }
  close();
  return "path";
}

void InternalDB::InsertUser() {
  if (connect()) { return; }
  close();
}

void InternalDB::DeleteUser() {
  if (connect()) { return; }
  close();
}

void InternalDB::ExistUser() {
  if (connect()) { return; }
  close();
}

void InternalDB::UpdateSyncFolder() {
  if (connect()) { return; }
  close();
}

void InternalDB::UpdatePassword() {
  if (connect()) { return; }
  close();
}

void InternalDB::SelectUserPassword() {
  if (connect()) { return; }
  close();
}

void InternalDB::InsertFile() {
  if (connect()) { return; }
  close();
}

void InternalDB::SelectFile() {
  if (connect()) { return; }
  close();
}

void InternalDB::UpdateFile() {
  if (connect()) { return; }
  close();
}

void InternalDB::InsertChunk() {
  if (connect()) { return; }
  close();
}

void InternalDB::SelectChunk() {
  if (connect()) { return; }
  close();
}

void InternalDB::UpdateChunk() {
  if (connect()) { return; }
  close();
}

void InternalDB::close() { }

bool InternalDB::dbExist() {
  return true;
}

int InternalDB::selectDeviceId() {
  if (connect()) { return -1; }
  close();
  return 1;
}

int InternalDB::selectUserId() {
  if (connect()) { return -1; }
  close();
  return 1;
}

std::string InternalDB::selectFolder() {
  if (connect()) { return ""; }
  close();
  return "path";
}
