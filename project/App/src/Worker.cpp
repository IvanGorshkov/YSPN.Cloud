#include "Worker.h"
#include <thread>
#include <boost/log/trivial.hpp>

void Worker::Run(std::queue<std::shared_ptr<BaseCommand>> &queue) {
  while (!queue.empty()) {
    BOOST_LOG_TRIVIAL(info) << "Worker: new command";
    auto command = queue.front();
    queue.pop();
    command->Do();
  }
}