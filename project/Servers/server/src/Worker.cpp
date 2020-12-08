#include "Worker.h"
#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

Worker::Worker(std::shared_ptr<CommandManager> manager,
               std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> get)
    : _manager(std::move(manager)),
      getConnection(std::move(get)) {

  BOOST_LOG_TRIVIAL(debug) << "Worker: create worker";
}

Worker::~Worker() {
  BOOST_LOG_TRIVIAL(debug) << "Worker: delete worker";
}

void Worker::Run() {
  BOOST_LOG_TRIVIAL(info) << "Worker: start worker";
  listening();
}

void Worker::listening() {
  while (true) {
    auto user = getConnection();
    if (user != nullptr) {
      BOOST_LOG_TRIVIAL(info) << "Worker: new request";
      onConnect(user);
    } else {
      BOOST_LOG_TRIVIAL(debug) << "Worker: sleep";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }
}

void Worker::onConnect(const std::shared_ptr<std::pair<UserSession, pt::ptree>> &user) {
  BOOST_LOG_TRIVIAL(info) << "Worker: start working with user id = " << user->first.GetUserId();

  auto responce = _manager->GetRequest(user->second);
  std::cout << "Put responce" << std::endl;

  BOOST_LOG_TRIVIAL(info) << "Worker: stop working with user id = " << user->first.GetUserId();
}
