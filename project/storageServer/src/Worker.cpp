#include "Worker.h"
#include <chrono>
#include <thread>

Worker::Worker(std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> get, bool &isWork)
    : getConnection(std::move(get)),
      _isWork(isWork) {
  BOOST_LOG_TRIVIAL(debug) << "Storage worker: create worker";
}

Worker::~Worker() {
  BOOST_LOG_TRIVIAL(debug) << "Storage worker: delete worker";
}

void Worker::Run() {
  BOOST_LOG_TRIVIAL(info) << "Storage worker: start worker";
  listening();
}

void Worker::listening() {
  while (_isWork) {
    auto user = getConnection();
    if (user != nullptr) {
      BOOST_LOG_TRIVIAL(info) << "Storage worker: new request";
      onConnect(user);
    } else {
      BOOST_LOG_TRIVIAL(debug) << "Storage worker: sleep";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }
}

void Worker::onConnect(const std::shared_ptr<std::pair<UserSession, pt::ptree>> &user) {
  BOOST_LOG_TRIVIAL(info) << "Storage worker: start working with user id = " << user->first.GetUserId();

  auto command = getCommand(user->second);
  if (command) {
    command->Do();
  }

  BOOST_LOG_TRIVIAL(info) << "Storage worker: stop working with user id = " << user->first.GetUserId();
}

Worker::Worker(const Worker &worker)
    : getConnection(worker.getConnection),
      _isWork(worker._isWork) {

  BOOST_LOG_TRIVIAL(debug) << "Storage worker: copy constructor";
}

//Worker::Worker(const Worker &&worker)
//    : getConnection(worker.getConnection),
//      _isWork(worker._isWork) {
//  BOOST_LOG_TRIVIAL(info) << "Storage worker: move constructor";
//}

std::unique_ptr<Command> Worker::getCommand(const pt::ptree &pt) {
  BOOST_LOG_TRIVIAL(debug) << "Storage worker: getCommand";
  std::string command;

  try {
    command = pt.get<std::string>("command");
  } catch (pt::ptree_error &er) {
    BOOST_LOG_TRIVIAL(warning) << "Storage worker: " << er.what();
    return nullptr;
  }

  auto sh = std::make_shared<pt::ptree>(pt);

//  if (command == "DownloadChunk") {
//    return std::make_unique<DownloadChunkCommand>(sh);
//  }
//
//  if (command == "UploadChunk") {
//    return std::make_unique<UploadChunkCommand>(sh);
//  }

  return nullptr;
}
