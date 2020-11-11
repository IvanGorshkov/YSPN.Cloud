#include "MetaDataDB.h"

MetaDataDB::MetaDataDB(): PostgreSQLDB() {
  if (!dbExist())  {
	CreateDB();
  }

  connect();
}

MetaDataDB::~MetaDataDB() {
  close();
}

void MetaDataDB::CreateDB() {

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