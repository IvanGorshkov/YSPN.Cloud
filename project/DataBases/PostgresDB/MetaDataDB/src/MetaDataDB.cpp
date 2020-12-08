#include "MetaDataDB.h"

MetaDataDB::MetaDataDB(std::string_view info): PostgresSQLDB(info) {
}

MetaDataDB& MetaDataDB::shared(std::string_view info) {
  static MetaDataDB shared(info);
  return shared;
}


MetaDataDB::~MetaDataDB() = default;

void MetaDataDB::InsertFile() {
  printf("Hello");
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
