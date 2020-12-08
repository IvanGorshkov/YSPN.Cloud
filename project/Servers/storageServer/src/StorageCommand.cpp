#include "StorageCommand.h"
#include <boost/log/trivial.hpp>

UploadChunkCommand::UploadChunkCommand(std::shared_ptr<pt::ptree> &request)
    : _request(request),
      _db(MongoDB::shared()),
      _chunk(*request) {
  BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: create command";
}

std::shared_ptr<pt::ptree> UploadChunkCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: Do";

  std::vector<Chunk> vec;
  try {
    vec = _chunk.GetChunk();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (vec.empty()) {
    BOOST_LOG_TRIVIAL(debug) << "UploadChunkCommand: vec empty";
    auto answer = SerializerAnswer(_chunk.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  _db.InsertChunk(vec);

//  for (auto &el : vec) {
//    std::cout << el.userId << std::endl;
//  }

  std::cout << "UploadChunkCommand: Work with Mongo" << std::endl;

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

  std::vector<UserChunk> vec;
  try {
    vec = _userChunk.GetChunk();
  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: " << er.what();
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Error in json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  if (vec.empty()) {
    BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: vec empty";
    auto answer = SerializerAnswer(_userChunk.GetRequestId(), "Empty json");
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

//  for (auto &el : vec) {
//    std::cout << el.userId << std::endl;
//  }

  auto ve1 = _db.GetChunk(vec);

  for (auto &&el: ve1) {
    std::cout << "UserId: " << el.userId << std::endl;
    std::cout << "ChunkId: " << el.chunkId << std::endl;
    std::cout << "rHash: " << el.rHash << std::endl;
    std::cout << "sHash: " << el.sHash << std::endl;
    std::cout << "data: " << el.data << std::endl;
    std::cout << std::endl;
  }

  std::cout << "DownloadChunkCommand: Work with Mongo" << std::endl;

  BOOST_LOG_TRIVIAL(debug) << "DownloadChunkCommand: do command stop";
  auto answer = SerializerChunk(_userChunk.GetRequestId(), ve1);
  return std::make_shared<pt::ptree>(answer.GetJson());
}