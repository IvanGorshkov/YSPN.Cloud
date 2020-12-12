#include "InternalDB.h"
#include "SQLiteQuery.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <utility>

InternalDB::InternalDB(std::string  databaseName): _databaseName(std::move(databaseName)) {
  if (connect()) {
    creatTable();
	_userId = selectUserId();
	_deviceId = selectDeviceId();
	_syncFolder = selectFolder();
	_lastUpdate = selectLastUpdate();
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
	std::cout << "Don't Insert" << std::endl;
  }
}

void InternalDB::InsertUser(const User& user) {
  if (!connect()) { return; }
  std::string query = "INSERT INTO User (user_id, login, password, device_id, device_name, sync_folder, last_update) VALUES ("
  	+ std::to_string(user.userId)
  	+ ", '" + user.login
  	+ "', '" + user.password
  	+"', " + std::to_string(user.deviceId)
  	+ ", '" + user.deviceName
  	+ "', '" + user.syncFolder
  	+ "', '" + user.lastUpdate
  	+ "');";
  insert(query);

  close();
}

void InternalDB::InsertChunk(const Chunks& chunks) {
  if (!connect()) { return; }
  std::string query = "INSERT INTO Chunks (id_file, chunk_size, rapid_hash, static_hash) VALUES (" + std::to_string(chunks.idFile) + ", " + std::to_string(chunks.chunkSize) + ", '" + chunks.rapidHash + "', '" +
	  chunks.staticHash + "');";
  insert(query);
  close();
}

void InternalDB::InsertFile(const std::vector<Files>& files) {
  if (!connect()) { return; }
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
  if (!connect()) { return; }
  std::string query = "DELETE FROM User WHERE user_id = " +  std::to_string(id) + ";";
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	std::cout << "Don't Deleted" << std::endl;
  }
  close();
}

//MARK: Обновление БД
bool InternalDB::update(const std::string& query) {
  auto pStmt = _stmt.get();
  sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	std::cout << "Don't Update" << std::endl;
	return false;
  }
  return true;
}

void InternalDB::UpdatePassword(const std::string& newPassword) {
  if (!connect()) { return; }
  std::string query = "Update User set password = \"" + newPassword + "\" where user_id = " +  std::to_string(_userId) + ";";
  update(query);
  close();
}

void InternalDB::UpdateSyncFolder(const std::string& newFolder) {
  if (!connect()) { return; }
  std::string query = "Update User set sync_folder = \"" + newFolder + "\" where user_id = " +  std::to_string(_userId) + ";";
  if (update(query)) {
	_syncFolder = newFolder;
  }
  close();
}

void InternalDB::UpdateFile() {
  if (!connect()) { return; }
  close();
}

void InternalDB::UpdateChunk() {
  if (!connect()) { return; }
  close();
}

//MARK: Проверка присудствия пользователя
bool InternalDB::ExistUser() {
  if (!connect()) { return false; }
  std::string query = "SELECT count(*) FROM User;";
  int count = selectId(query);
  close();
  return count != 0;
}

//MARK: Получение данных из БД
std::string InternalDB::SelectUserPassword() {
  if (!connect()) { return ""; }
  std::string query = "Select password from User where user_id = " +  std::to_string(_userId) + ";";
  std::string password = selectStr(query);
  close();
  return  password;
}

Files InternalDB::SelectFile(size_t idFile) {
  Files file;
  if (!connect()) { return file; }
  std::string query = "SELECT * FROM Files Where id = " + std::to_string(idFile) + ";";
  sqlite3_exec(_database.get(), query.c_str(), callbackFile, &file, nullptr);
  close();
  return file;
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
  }
  return  id;
}

int InternalDB::selectDeviceId() {
  std::string query = "SELECT device_id FROM User;";
  int id = selectId(query);
  return id;
}

int InternalDB::selectUserId() {
  std::string query = "SELECT user_id FROM User;";
  int id = selectId(query);
  return id;
}

std::string InternalDB::selectFolder() {
  std::string query = "SELECT sync_folder FROM User;";
  std::string folder = selectStr(query);
  return folder;
}

std::string InternalDB::selectLastUpdate() {
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
  }
  return string;
}

//MARK: callback для заполнения структуры
int InternalDB::callbackFile(void* data, int argc, char** argv, char** azColName) {
  auto* file = (Files *)data;
  for (int i = 0; i < argc; i++) {
	std::string col(azColName[i]);
	if (col == "id") {
	  (*file).id = boost::lexical_cast<int>(argv[i]);
	}
	if (col == "file_name") {
	  (*file).file_name = argv[i];
	}
	if (col == "file_extention") {
	  (*file).file_extention = argv[i];
	}
	if (col == "file_size") {
	  (*file).file_size = boost::lexical_cast<int>(argv[i]);
	}
	if (col == "file_path") {
	  (*file).file_path = argv[i];
	}
	if (col == "count_chunks") {
	  (*file).count_chunks = boost::lexical_cast<int>(argv[i]);
	}
	if (col == "is_download") {
	  (*file).is_download = argv[i];
	}
	if (col == "create_date") {
	  (*file).create_date = argv[i];
	}
	if (col == "update_date") {
	  (*file).update_date = argv[i];
	}
  }

  return 0;
}

//MARK: Создание Последней даты использования
void InternalDB::SaveLastUpdate() {
  if (!connect()) { return; }
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
	std::cout << "Didn't Create Table Files!" << std::endl;
  }
  sqlite3_prepare(_database.get(), createQueryChunks.c_str(), createQueryChunks.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	std::cout << "Didn't Create Table Chunks!" << std::endl;
  }
  sqlite3_prepare(_database.get(), createQueryUser.c_str(), createQueryUser.size(), &pStmt, nullptr);
  _stmt.reset(pStmt);
  if (sqlite3_step(_stmt.get()) != SQLITE_DONE) {
	std::cout << "Didn't Create Table User!" << std::endl;
  }
}

//MARK: Подключение к Базе Данных
bool InternalDB::connect() {
  auto pDB = _database.get();
  if (SQLITE_OK != sqlite3_open_v2(_databaseName.c_str(), &pDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
	printf("Failed to open conn:\n");
	return false;
  }
  _database.reset(pDB);
  return true;
}

//MARK: Отключение  Базы Данных
void InternalDB::close() {
  sqlite3_shutdown();
}