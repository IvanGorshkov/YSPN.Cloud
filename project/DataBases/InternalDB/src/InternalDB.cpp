#include <boost/log/trivial.hpp>
#include "InternalDB.h"
#include "SQLiteQuery.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <utility>
#include "InternalExceptions.h"

InternalDB::InternalDB(std::string  databaseName): _databaseName(std::move(databaseName)),
												   _userId(-1),
												   _deviceId(-1),
												   _syncFolder(""),
												   _lastUpdate("") {
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Init DB";
  if (connect()) {
    creatTable();
    if (existUser()) {
	  _userId = selectUserId();
	  _deviceId = selectDeviceId();
	  _syncFolder = selectFolder();
	  _lastUpdate = selectLastUpdate();
	}
	close();
  }
}

//MARK: Getters параметров
int InternalDB::GetUserId() const {
  return _userId;
}

int InternalDB::GetDeviceId() const {
  return  _deviceId;
}

std::string InternalDB::GetSyncFolder() const {
  return _syncFolder;
}

std::string InternalDB::GetLastUpdate() {
  _lastTMPUpdate = boost::posix_time::second_clock::universal_time();
  return _lastUpdate;
}

//MARK: Занесение в таблицу
void InternalDB::insert(const std::string& query) {
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	BOOST_LOG_TRIVIAL(error) << "InternalDB: Don't Insert";
	throw InternalExceptions("Don't insert");
  }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Inserted";
}

void InternalDB::InsertUser(const User& user) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Insert User";
  std::string query = "INSERT INTO User (user_id, login, password, device_id, device_name, sync_folder, last_update) VALUES ("
  	+ std::to_string(user.userId)
  	+ ", '" + user.login
  	+ "', '" + user.password
  	+"', " + std::to_string(user.deviceId)
  	+ ", '" + user.deviceName
  	+ "', '" + user.syncFolder
  	+ "', '1970-Dec-31 12:30:02'"
  	+ "');";
  insert(query);
  _userId = user.userId;
  _deviceId = user.deviceId;
  _syncFolder = user.syncFolder;
  _lastUpdate = "1970-Dec-31 12:30:02";
  close();
}

void InternalDB::InsertChunk(const Chunks& chunks) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Insert Chunck";
  std::string query = "INSERT INTO Chunks (id_file, chunk_size, rapid_hash, static_hash) VALUES (" + std::to_string(chunks.idFile) + ", " + std::to_string(chunks.chunkSize) + ", '" + chunks.rapidHash + "', '" +
	  chunks.staticHash + "');";
  insert(query);
  close();
}

void InternalDB::InsertFile(const std::vector<Files>& files) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Insert Files";
  for(const auto& file: files) {
	std::string query = "INSERT INTO Files (file_name, file_extention, file_size, file_path, count_chunks, version, is_download, update_date, create_date) VALUES ('" + file.file_name + "', '" + file.file_extention + "', " + std::to_string(file.file_size) + ", '" +
		file.file_path + "', " + std::to_string(file.count_chunks) + ", " + std::to_string(file.version) + ", " + std::to_string(file.is_download.value()) + ", '" +
		file.update_date + "', '" + file.create_date + "');";
	insert(query);
  }
  close();
}

//MARK: Удаление из БД
void InternalDB::DeleteUser(size_t id) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "DELETE FROM User WHERE user_id = " +  std::to_string(id) + ";";
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	BOOST_LOG_TRIVIAL(error) << "InternalDB: Don't Deleted";
  }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Deleted";
  close();
}

//MARK: Обновление БД
bool InternalDB::update(const std::string& query) {
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	BOOST_LOG_TRIVIAL(error) << "InternalDB: Don't Update";
	return false;
  }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Updated";
  return true;
}

void InternalDB::UpdatePassword(const std::string& newPassword) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "Update User set password = \"" + newPassword + "\" where user_id = " +  std::to_string(_userId) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to update password";
  update(query);
  close();
}

void InternalDB::UpdateSyncFolder(const std::string& newFolder) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "Update User set sync_folder = \"" + newFolder + "\" where user_id = " +  std::to_string(_userId) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to update sync folder";
  if (update(query)) {
	_syncFolder = newFolder;
  }
  close();
}

void InternalDB::UpdateFile() {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  close();
}

void InternalDB::UpdateChunk() {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  close();
}

//MARK: Проверка присудствия пользователя
bool InternalDB::existUser() {
  std::string query = "SELECT count(*) FROM User;";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Check exist user";
  int count = selectId(query);
  return count != 0;
}

//MARK: Получение данных из БД
std::string InternalDB::SelectUserPassword() {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "Select password from User where user_id = " +  std::to_string(_userId) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to select user password";
  std::string password = selectStr(query);
  close();
  return  password;
}

Files InternalDB::SelectFile(size_t idFile) {
  Files file;
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "SELECT * FROM Files Where id = " + std::to_string(idFile) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Select File by id = " << std::to_string(idFile);
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);

  if (sqlite3_step(_stmt.get()) == SQLITE_ROW) {
    file = getOneFile();
	BOOST_LOG_TRIVIAL(debug) << "InternalDB: Selected";
  } else {
	BOOST_LOG_TRIVIAL(error) << "File by id = " + std::to_string(idFile) + " don't exist";;
    throw InternalExceptions("File by id = " + std::to_string(idFile) + " don't exist");
  }

  close();
  return file;
}

Files InternalDB::getOneFile() {
  Files file;
  file.id = sqlite3_column_int(_stmt.get(), 0);
  file.file_name = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 1));
  file.file_extention = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 2));
  file.file_size = sqlite3_column_int(_stmt.get(), 3);
  file.file_path = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 4));
  file.count_chunks = sqlite3_column_int(_stmt.get(), 5);
  file.version = sqlite3_column_int(_stmt.get(), 6);
  file.is_download = sqlite3_column_int(_stmt.get(), 7);
  file.update_date = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 8));
  file.create_date = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 9));
  return file;
}

std::vector<Files> InternalDB::SelectAllFiles() {
  std::vector<Files> files;
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "SELECT * FROM Files;";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Select All Files";
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  while(sqlite3_step(_stmt.get()) == SQLITE_ROW) {
	files.push_back(getOneFile());
	BOOST_LOG_TRIVIAL(debug) << "InternalDB: Selected";
  }
  close();
  return files;
}

void InternalDB::SelectChunk() {
  if (!connect()) { return; }
  close();
}

int InternalDB::selectId(const std::string& query) {
  int id = -1;
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  while(sqlite3_step(_stmt.get()) == SQLITE_ROW) {
	id = sqlite3_column_int(_stmt.get(), 0);
	BOOST_LOG_TRIVIAL(debug) << "InternalDB: Selected";
  }
  return  id;
}

int InternalDB::selectDeviceId() {
  std::string query = "SELECT device_id FROM User;";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to select device_id";
  int id = selectId(query);
  return id;
}

int InternalDB::selectUserId() {
  std::string query = "SELECT user_id FROM User;";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to select user_id";
  int id = selectId(query);
  return id;
}

std::string InternalDB::selectFolder() {
  std::string query = "SELECT sync_folder FROM User;";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to select sync_folder";
  std::string folder = selectStr(query);
  return folder;
}

std::string InternalDB::selectLastUpdate() {
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to select last update";
  std::string query = "SELECT last_update FROM User;";
  std::string date = selectStr(query);
  return  date;
}

std::string InternalDB::selectStr(const std::string& query) {
  std::string string;
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  while(sqlite3_step(_stmt.get()) == SQLITE_ROW) {
	string =  boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 0));
	BOOST_LOG_TRIVIAL(debug) << "InternalDB: Selected";
  }
  return string;
}

//MARK: Создание Последней даты использования
void InternalDB::SaveLastUpdate() {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to save last update";
  std::string query = "Update User set last_update = \"" + boost::lexical_cast<std::string>(_lastTMPUpdate) + "\" where user_id = " +  std::to_string(_userId) + ";";
  update(query);
  _lastUpdate = boost::lexical_cast<std::string>(_lastTMPUpdate);
  close();
}

//MARK: Создание табицы
void InternalDB::creatTable() {
  auto pStmt = _stmt.get();
  sqlite3_prepare(_database.get(), createQueryFiles.c_str(), createQueryFiles.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
    std::string error = "InternalDB: Didn't Create Table Files";
	BOOST_LOG_TRIVIAL(error) << error;
	throw InternalExceptions(error);
  }
  sqlite3_prepare(_database.get(), createQueryChunks.c_str(), createQueryChunks.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	std::string error = "InternalDB: Didn't Create Table Chunks";
	BOOST_LOG_TRIVIAL(error) << error;
	throw InternalExceptions(error);
  }
  sqlite3_prepare(_database.get(), createQueryUser.c_str(), createQueryUser.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	std::string error = "InternalDB: Didn't Create Table Users";
	BOOST_LOG_TRIVIAL(error) << error;
	throw InternalExceptions(error);
  }
}

//MARK: Подключение к Базе Данных
bool InternalDB::connect() {
  auto pDB = _database.get();
  if (SQLITE_OK != sqlite3_open_v2(_databaseName.c_str(), &pDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
	BOOST_LOG_TRIVIAL(error) << "InternalDB: Failed to connect";
	return false;
  }
  _database.reset(pDB);
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Connect";
  return true;
}

//MARK: Отключение  Базы Данных
void InternalDB::close() {
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Close";
  sqlite3_shutdown();
}