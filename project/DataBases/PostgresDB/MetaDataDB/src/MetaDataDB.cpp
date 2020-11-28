#include "MetaDataDB.h"

MetaDataDB::MetaDataDB(): PostgreSQLDB() {
  if (!dbExist())  {
	createDB();
  }

  connect();
}

MetaDataDB::~MetaDataDB() {
  close();
}

void MetaDataDB::InsertFile() {
  if (!connect()) { return; }
}

void MetaDataDB::InsertChunk() {
  if (!connect()) { return; }
}

void MetaDataDB::InsertFileChunk() {
  if (!connect()) { return; }
}

void MetaDataDB::SelectFile() {
  if (!connect()) { return; }
}

void MetaDataDB::SelectChunk() {
  if (!connect()) { return; }
}

void MetaDataDB::SelectFileChunk() {
  if (!connect()) { return; }
}

void MetaDataDB::SelectFilesByUser() {
  if (!connect()) { return; }
}

void MetaDataDB::createDB() {

}
