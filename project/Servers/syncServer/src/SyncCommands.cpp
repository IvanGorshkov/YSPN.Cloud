#include "SyncCommands.h"
#include <boost/log/trivial.hpp>
#include "TestPostgra.h"

DataUpdateCommand::DataUpdateCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _db(MetaDataDB::shared("user=ysnp dbname=ysnpcloud")),
      _userDate(*request) {
  BOOST_LOG_TRIVIAL(debug) << "DataUpdateCommand: create command";
}

std::shared_ptr<pt::ptree> DataUpdateCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DataUpdateCommand: Do";

  try {
    _db.Connect();
    BOOST_LOG_TRIVIAL(info) << "DataUpdateCommand: connect to database";

  } catch (PostgresExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "DataUpdateCommand: " << er.what();
    auto answer = SerializerAnswer(_userDate.GetRequestId(), "Database error");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  UserDate requestUserDate;
  try {
    requestUserDate = _userDate.GetUserDate();
    BOOST_LOG_TRIVIAL(info) << "DataUpdateCommand: parse json, user = " << requestUserDate.userId;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(error) << "DataUpdateCommand: " << er.what();
    auto answer = SerializerAnswer(_userDate.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  std::vector<FileInfo> responseFileInfo;
  // TODO DataUpdateCommand get FileInfo vector

  try {
	responseFileInfo = _db.GetUserFilesByTime(requestUserDate);
    BOOST_LOG_TRIVIAL(info) << "DataUpdateCommand: get files from database";

  } catch (PostgresExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "DataUpdateCommand: " << er.what();
    auto answer = SerializerAnswer(_userDate.GetRequestId(), "Fail to get file");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }


  BOOST_LOG_TRIVIAL(info) << "DataUpdateCommand: Status Ok";
  auto answer = SerializerFileInfo(_userDate.GetRequestId(), responseFileInfo);
  return std::make_shared<pt::ptree>(answer.GetJson());
}

UploadFileCommand::UploadFileCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _db(MetaDataDB::shared("user=ysnp dbname=ysnpcloud")),
      _fileInfo(*request) {
  BOOST_LOG_TRIVIAL(debug) << "UploadFileCommand: create command";
}

std::shared_ptr<pt::ptree> UploadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "UploadFileCommand: Do";

  try {
    _db.Connect();
    BOOST_LOG_TRIVIAL(info) << "UploadFileCommand: connect to database";

  } catch (PostgresExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadFileCommand: " << er.what();
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Database error");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  std::vector<FileInfo> requestFileInfo;
  try {
    requestFileInfo = _fileInfo.GetFileInfo();
    BOOST_LOG_TRIVIAL(info) << "UploadFileCommand: parse json, user = " << requestFileInfo[0].userId;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadFileCommand: " << er.what();
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (requestFileInfo.empty()) {
    BOOST_LOG_TRIVIAL(error) << "UploadFileCommand: empty chunk vector";
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  try {
    _db.InsertFile(requestFileInfo[0]);
    BOOST_LOG_TRIVIAL(info) << "UploadFileCommand: insert file to database";

  } catch (PostgresExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadFileCommand: " << er.what();
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Fail to insert file");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  BOOST_LOG_TRIVIAL(info) << "UploadFileCommand: Status Ok";
  auto answer = SerializerAnswer(_fileInfo.GetRequestId());
  return std::make_shared<pt::ptree>(answer.GetJson());
}