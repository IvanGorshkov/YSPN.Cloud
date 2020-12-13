#include "Worker.h"

#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>
#include <utility>

Worker::Worker(std::shared_ptr<CommandManager> manager,
               std::shared_ptr<NetworkSever> server)
    : _manager(std::move(manager)),
      _networkServer(std::move(server)) {
  BOOST_LOG_TRIVIAL(debug) << "Worker: create worker";
}

//Worker::Worker(std::shared_ptr<CommandManager> manager,
//               std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> get)
//    : _manager(std::move(manager)),
//      getConnection(std::move(get)) {
//
//  BOOST_LOG_TRIVIAL(debug) << "Worker: create worker";
//}

Worker::~Worker() {
  BOOST_LOG_TRIVIAL(debug) << "Worker: delete worker";
}

void Worker::Run() {
  BOOST_LOG_TRIVIAL(info) << "Worker: start worker";
  listening();
}

void Worker::listening() {
  while (true) {
    auto request = _networkServer->GetRequest();
    if (request != nullptr) {
      BOOST_LOG_TRIVIAL(info) << "Worker: new request";
      onConnect(request);
    } else {
      BOOST_LOG_TRIVIAL(debug) << "Worker: sleep";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }
}

void Worker::onConnect(const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, pt::ptree>> &request) {
  BOOST_LOG_TRIVIAL(info) << "Worker: start working with user id = ";

  auto responce = _manager->GetRequest(request->second);
  _networkServer->PutResponce(std::make_shared<std::pair<std::shared_ptr<UserSession>,
                                                         pt::ptree>>(request->first, *responce));

  BOOST_LOG_TRIVIAL(info) << "Worker: stop working with user id = ";
}
