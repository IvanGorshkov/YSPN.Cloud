#include "MetaDataDB.h"
#include "PostgresExceptions.h"
#include <boost/lexical_cast.hpp>

MetaDataDB::MetaDataDB(std::string_view info): PostgresSQLDB(info) {
}

MetaDataDB& MetaDataDB::shared(std::string_view info) {
  static MetaDataDB shared(info);
  return shared;
}

MetaDataDB::~MetaDataDB() = default;

void MetaDataDB::InsertFile(const FileInfo& fileMeta) {
  try {
  	pqExec("begin;", PostgresExceptions("invalid to start transaction")); // Начало транзакции
  	pqExec("savepoint f_savepoint;", PostgresExceptions("invalid to update")); // Точка сохранения

  	std::string query = "Update Files set is_current = 'f' where id_file_for_user = "
  		+ std::to_string(fileMeta.file.fileId)
  		+ "and id_user = "
  		+ std::to_string(fileMeta.userId) + ";";
  	pqExec(query, PostgresExceptions("invalid to update")); // Снятие старой версии

  	query = "INSERT INTO Files (id_file_for_user, id_user, file_name, file_extention, file_size, count_chunks, version, is_current, is_deleted, update_date, create_date) VALUES ("
  		+ std::to_string(fileMeta.file.fileId)
  		+ ", " + std::to_string(fileMeta.userId)
  		+ ", '" + fileMeta.file.fileName
  		+ "', '" + fileMeta.file.fileExtension
  		+ "', " + std::to_string(fileMeta.file.fileSize)
  		+ ", " +std::to_string(fileMeta.file.chunksCount)
  		+ ", " +std::to_string(fileMeta.file.version)
  		+ ", '" +std::to_string(fileMeta.file.isCurrent.value())
  		+ "', '"+std::to_string(fileMeta.file.isDeleted.value())
  		+ "', " +"now(), now());";
  	pqExec(query, PostgresExceptions("invalid to insert data to db")); // Добавление нового файла

  	for (const auto& chunk: fileMeta.chunkMeta) {
		query = "INSERT INTO Chunks (id_chunk_for_user, id_user) VALUES ("
			+std::to_string(chunk.chunkId)+","
			+ std::to_string(fileMeta.userId) +");";
		pqExec(query, PostgresExceptions("invalid to insert data to db")); // Добавление нового чанка
  	}

  	query = "SELECT id FROM Files Where id_file_for_user = "
  		+ std::to_string(fileMeta.file.fileId)
  		+ "and id_user = "
  		+ std::to_string(fileMeta.userId)
  		+ " ORDER  BY  id  DESC Limit 1;";

  	int id = getLastIdOfFileUser(query, PostgresExceptions("invalid to select id"));
  	for (const auto& version: fileMeta.fileChunksMeta) {
		query = "INSERT INTO Version (id_file , id_chunk, id_order) VALUES ("
			+ std::to_string(id) + ", "
			+ std::to_string(version.chunkId) + ", "
			+ std::to_string(version.chunkOrder) + ");";
		pqExec(query, PostgresExceptions("invalid to insert data to db")); // Добавление новой версии
  	}

  	pqExec("commit;", PostgresExceptions("invalid to end transation")); // Завершение транзакции
  }
  catch (PostgresExceptions& exceptions) {
	throw exceptions;
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

int MetaDataDB::getLastIdOfFileUser(const std::string &query, PostgresExceptions exceptions) {
  PGresult *res = PQexec(_conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
	PQexec(_conn,"rollback to savepoint f_savepoint;");
	PQclear(res);
	throw exceptions;
  }
  int id = boost::lexical_cast<int>(PQgetvalue(res, 0, 0));
  PQclear(res);
  return id;
}
