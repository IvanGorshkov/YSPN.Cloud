#include "MetaDataDB.h"
#include "PostgresExceptions.h"

MetaDataDB::MetaDataDB(std::string_view info): PostgresSQLDB(info) {
}

MetaDataDB& MetaDataDB::shared(std::string_view info) {
  static MetaDataDB shared(info);
  return shared;
}

MetaDataDB::~MetaDataDB() = default;

void MetaDataDB::InsertFile(const FileMeta& fileMeta) const {
  std::string query = "INSERT INTO Files (id_file_for_user, id_user, file_name, file_extention, file_size, count_chunks, version, is_current, update_date, create_date) VALUES (" + std::to_string(fileMeta.fileId) + ", " + std::to_string(fileMeta.userId) + ", '" +fileMeta.fileName + "', '" + fileMeta.fileExtension +"', " + std::to_string(fileMeta.fileSize) + ", " +std::to_string(fileMeta.chunksCount) + ", " +std::to_string(fileMeta.version) + ", '" +std::to_string(fileMeta.isCurrent) + "', " +"now(), now());";
  PGresult *res = PQexec(_conn, query.c_str());
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	throw PostgresExceptions("invalid to insert data to db");
  }
}

void MetaDataDB::InsertChunk() {
}

void MetaDataDB::InsertFileChunk() {
}

void MetaDataDB::SelectFile() {
}

void MetaDataDB::SelectChunk() {
}

void MetaDataDB::SelectFileChunkVersion() {
}

void MetaDataDB::SelectFilesByUser() {
}
