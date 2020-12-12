#include "Worker.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <boost/log/trivial.hpp>

Worker::Worker(const std::queue<std::shared_ptr<CommandInterface>> &queue)
    : _commands(queue) {
  BOOST_LOG_TRIVIAL(debug) << "Worker: create worker";
}

void Worker::Run() {
  BOOST_LOG_TRIVIAL(info) << "Worker: start worker";
  listening();
}

void Worker::listening() {
  while (true) {
    if (!_commands.empty()) {
      BOOST_LOG_TRIVIAL(info) << "Worker: new command";
      auto command = _commands.front();
      _commands.pop();
      command->Do();
    } else {
//      BOOST_LOG_TRIVIAL(debug) << "Worker: sleep";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }
}