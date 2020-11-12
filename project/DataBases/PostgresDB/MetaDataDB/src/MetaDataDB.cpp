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

}

void MetaDataDB::InsertChunk() {

}

void MetaDataDB::InsertFileChunk() {

}

void MetaDataDB::SelectFile() {

}

void MetaDataDB::SelectChunk() {

}

void MetaDataDB::SelectFileChunk() {

}

void MetaDataDB::SelectFilesByUser() {

}
void MetaDataDB::createDB() {

}
