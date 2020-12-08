#include "StorageManager.h"
#include <boost/log/trivial.hpp>

std::shared_ptr<pt::ptree> StorageManager::GetRequest(pt::ptree &val) {
  std::unique_ptr<Command> command;
  int requestId = 0;

  try {
    requestId = val.get<int>("requestId");
  } catch (pt::ptree_error &er) {
    BOOST_LOG_TRIVIAL(warning) << "Storage worker: " << er.what();
    auto answer = SerializerAnswer(-1, "No requestId");
  }

  try {
    command = getCommand(val);

  } catch (NoCommand &er) {
    BOOST_LOG_TRIVIAL(warning) << "Storage worker: " << er.what();
    auto answer = SerializerAnswer(requestId, er.what());
    return std::make_shared<pt::ptree>(answer.GetJson());

  } catch (WrongCommand &er) {
    BOOST_LOG_TRIVIAL(warning) << "Storage worker: " << er.what();
    auto answer = SerializerAnswer(requestId, er.what());
    return std::make_shared<pt::ptree>(answer.GetJson());
  }

  auto request = command->Do();
  return request;
}

std::unique_ptr<Command> StorageManager::getCommand(const pt::ptree &val) {
  BOOST_LOG_TRIVIAL(debug) << "Storage worker: getCommand";
  std::string command;

  try {
    command = val.get<std::string>("command");
  } catch (pt::ptree_error &er) {
    throw NoCommand(er.what());
  }

  auto sh = std::make_shared<pt::ptree>(val);

  if (command == "DownloadChunk") {
    return std::make_unique<DownloadChunkCommand>(sh);
  }

  if (command == "UploadChunk") {
    return std::make_unique<UploadChunkCommand>(sh);
  }

  throw WrongCommand("Wrong command");
}