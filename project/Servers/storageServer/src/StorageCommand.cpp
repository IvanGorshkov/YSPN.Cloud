#include "StorageCommand.h"
#include <boost/log/trivial.hpp>

//void printChunkVec(const std::vector<Chunk> &vec) {
//  BOOST_LOG_TRIVIAL(debug) << "Print Vector";
//
//  for (auto &&el: vec) {
//    std::cout << "UserId: " << el.userId << std::endl;
//    std::cout << "ChunkId: " << el.chunkId << std::endl;
//    std::cout << "rHash: " << el.rHash << std::endl;
//    std::cout << "sHash: " << el.sHash << std::endl;
//    std::cout << "data: " << el.data << std::endl;
//    std::cout << std::endl;
//  }
//}

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
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: vec empty";
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  try {
    _db.InsertChunk(requestVector);
  } catch (MongoExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Fail to read Chunk");
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
    BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: requestVector empty";
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  std::vector<Chunk> responceVector;

  try {
    responceVector = _db.GetChunk(requestVector);
  } catch (MongoExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "DownloadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Fail to read Chunk");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

//  printChunkVec(responceVector);

  auto answer = SerializerChunk(_userChunk.GetRequestId(), responceVector);
  return std::make_shared<pt::ptree>(answer.GetJson());
}