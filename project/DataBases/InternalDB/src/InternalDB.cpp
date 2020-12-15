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

bool InternalDB::IsFileExist(const int idFile) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "SELECT count(*) FROM Files Where id="+ std::to_string(idFile) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Check exist file id=" + std::to_string(idFile);
  int count = selectId(query);
  close();
  return count != 0;
}

std::vector<UserChunk> InternalDB::GetUsersChunks(const int idFile) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
	std::vector<UserChunk> userChunks;
  	std::string query = "SELECT id FROM Chunks Where id_file = " + std::to_string(idFile) + ";";
  	auto pStmt = _stmt.get();
  	sqlite3_prepare_v2(_database.get(), query.c_str(), query.size(), &pStmt, nullptr);
  	_stmt.reset(pStmt);
    while(sqlite3_step(_stmt.get()) == SQLITE_ROW) {
   	 userChunks.emplace_back(UserChunk{_userId, sqlite3_column_int(_stmt.get(), 0)});
   	 BOOST_LOG_TRIVIAL(debug) << "InternalDB: Selected";
  	}
  close();
  return userChunks;
}

void InternalDB::InsertFileInfo(const std::vector<FileInfo>& filesInfo) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
	for (const auto& fileInfo: filesInfo) {
	  insertOneFile(fileInfo.file);
	  int id = selectId("SELECT id FROM Files ORDER  BY  id  DESC Limit 1");
	  for (const auto& fileChunksMeta: fileInfo.fileChunksMeta) {
		InsertChunk(fileChunksMeta, id);
	  }
	}
  close();
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

UserDate InternalDB::GetLastUpdate() {
  _lastTMPUpdate = boost::posix_time::second_clock::universal_time();
  return UserDate{.userId = _userId, .date = _lastUpdate};
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

void InternalDB::InsertChunk(const FileChunksMeta& chunks, const int idFile) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Insert Chunck";
  std::string query = "INSERT INTO Chunks (id_file, chunk_order) VALUES (" + std::to_string(idFile) + ", " + std::to_string(chunks.chunkOrder) + ");";
  insert(query);
  close();
}

void InternalDB::insertOneFile(const FileMeta& file) {
  std::string query = "INSERT INTO Files "
					  "(file_name, file_extention, file_size, "
	   "file_path, count_chunks, version, is_download, "
	"update_date, create_date) VALUES ('"
	+ file.fileName + "', '" + file.fileExtension
	+ "', " + std::to_string(file.fileSize)
	+ ", '" + file.filePath + "', " +
	std::to_string(file.chunksCount) + ", "
	+ std::to_string(file.version) + ", "
	+ std::to_string(file.isDownload.value()) + ", '"
	+ file.updateDate + "', '" + file.createDate + "');";
  insert(query);
}

void InternalDB::InsertFile(const std::vector<FileMeta>& files) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Insert Files";
  for(const auto& file: files) {
	insertOneFile(file);
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

FileMeta InternalDB::SelectFile(size_t idFile) {
  FileMeta file;
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

FileMeta InternalDB::getOneFile() {
  FileMeta file;
  file.fileId = sqlite3_column_int(_stmt.get(), 0);
  file.fileName = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 1));
  file.fileExtension = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 2));
  file.fileSize = sqlite3_column_int(_stmt.get(), 3);
  file.filePath = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 4));
  file.chunksCount = sqlite3_column_int(_stmt.get(), 5);
  file.version = sqlite3_column_int(_stmt.get(), 6);
  file.isDownload = sqlite3_column_int(_stmt.get(), 7);
  file.updateDate = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 8));
  file.createDate = boost::lexical_cast<std::string>(sqlite3_column_text(_stmt.get(), 9));
  return file;
}

std::vector<FileMeta> InternalDB::SelectAllFiles() {
  std::vector<FileMeta> files;
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

//MARK: Функции для регистрации
void InternalDB::UpdatePassword(const std::string& newPassword) {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "Update User set password = \"" + newPassword + "\" where user_id = " +  std::to_string(_userId) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to update password";
  update(query);
  close();
}
std::string InternalDB::SelectUserPassword() {
  if (!connect()) { throw InternalExceptions("Don't connect"); }
  std::string query = "Select password from User where user_id = " +  std::to_string(_userId) + ";";
  BOOST_LOG_TRIVIAL(debug) << "InternalDB: Prepare to select user password";
  std::string password = selectStr(query);
  close();
  return  password;
}