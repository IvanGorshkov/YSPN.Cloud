#include "SyncCommands.h"
#include <boost/log/trivial.hpp>

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

  FileMeta responseFileMeta;
  try {
    // Get fileMeta
  } catch (std::exception &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userDate.GetRequestId(), "Fail to get file");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  auto answer = SerializerFileMeta(_userDate.GetRequestId(), requestUserDate.userId, responseFileMeta);
  return std::make_shared<pt::ptree>(answer.GetJson());
}

UploadFileCommand::UploadFileCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _fileMeta(*request) {
  BOOST_LOG_TRIVIAL(debug) << "UploadFileCommand: create command";

  // TODO UploadFileCommand constructor PostgreSQL
}

std::shared_ptr<pt::ptree> UploadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "UploadFileCommand: Do";
  // TODO UploadFileCommand Do PostgreSQL

  FileMeta requestFileMeta;
  try {
    requestFileMeta = _fileMeta.GetFileMeta();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_fileMeta.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (requestFileMeta.fileChunksMeta.empty()) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: empty chunk vector";
    auto answer = SerializerAnswer(_fileMeta.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  try {
    // Insert fileMeta
  } catch (std::exception &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_fileMeta.GetRequestId(), "Fail to insert file");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  auto answer = SerializerAnswer(_fileMeta.GetRequestId());
  return std::make_shared<pt::ptree>(answer.GetJson());
}