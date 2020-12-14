#include "StorageCommands.h"
#include <boost/log/trivial.hpp>

UploadChunkCommand::UploadChunkCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _db(MongoDB::shared()),
      _chunk(*request) {
  BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: create command";
}

std::shared_ptr<pt::ptree> UploadChunkCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: Do";

  std::vector<Chunk> requestVector;
  try {
    requestVector = _chunk.GetChunk();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (requestVector.empty()) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: empty chunk vector";
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  try {
    _db.InsertChunk(requestVector);
  } catch (MongoExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Fail to insert Chunk");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  auto answer = SerializerAnswer(_chunk.GetRequestId());
  return std::make_shared<pt::ptree>(answer.GetJson());
}

DownloadChunkCommand::DownloadChunkCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _db(MongoDB::shared()),
      _userChunk(*request) {
  BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: create command";
}

std::shared_ptr<pt::ptree> DownloadChunkCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: Do";

  std::vector<UserChunk> requestVector;
  try {
    requestVector = _userChunk.GetChunk();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (requestVector.empty()) {
    BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: empty user chunk vector";
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  std::vector<Chunk> responseVector;
  try {
    responseVector = _db.GetChunk(requestVector);
  } catch (MongoExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "DownloadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Fail to get Chunk");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  auto answer = SerializerChunk(_userChunk.GetRequestId(), responseVector);
  return std::make_shared<pt::ptree>(answer.GetJson());
}