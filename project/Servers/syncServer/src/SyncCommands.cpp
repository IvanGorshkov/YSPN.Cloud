#include "SyncCommands.h"
#include <boost/log/trivial.hpp>
#include "TestPostgra.h"

DataUpdateCommand::DataUpdateCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _userDate(*request) {
  BOOST_LOG_TRIVIAL(debug) << "DataUpdateCommand: create command";

  // TODO DataUpdateCommand constructor PostgreSQL
}

std::shared_ptr<pt::ptree> DataUpdateCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DataUpdateCommand: Do";
  // TODO DataUpdateCommand Do PostgreSQL

  UserDate requestUserDate;
  try {
    requestUserDate = _userDate.GetUserDate();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userDate.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  std::vector<FileInfo> responseFileInfo;
  try {
    // Get fileMeta
  } catch (std::exception &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userDate.GetRequestId(), "Fail to get file");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  // test
  auto &testPostgra = TestPostgra::GetInstance();
  responseFileInfo = testPostgra.GetFileInfo();
  // test

  auto answer = SerializerFileInfo(_userDate.GetRequestId(), responseFileInfo);
  return std::make_shared<pt::ptree>(answer.GetJson());
}

UploadFileCommand::UploadFileCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _fileInfo(*request) {
  BOOST_LOG_TRIVIAL(debug) << "UploadFileCommand: create command";

  // TODO UploadFileCommand constructor PostgreSQL
}

std::shared_ptr<pt::ptree> UploadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "UploadFileCommand: Do";
  // TODO UploadFileCommand Do PostgreSQL

  std::vector<FileInfo> requestFileInfo;
  try {
    requestFileInfo = _fileInfo.GetFileInfo();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (requestFileInfo.empty()) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: empty chunk vector";
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  try {
    // Insert fileInfo
  } catch (std::exception &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_fileInfo.GetRequestId(), "Fail to insert file");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  // test
  auto &testPostgra = TestPostgra::GetInstance();
  testPostgra.SetFileInfo(requestFileInfo);
  // test

  auto answer = SerializerAnswer(_fileInfo.GetRequestId());
  return std::make_shared<pt::ptree>(answer.GetJson());
}