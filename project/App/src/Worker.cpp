#include "Worker.h"
#include <thread>
#include <boost/log/trivial.hpp>

//Worker::Worker(std::shared_ptr<std::queue<std::shared_ptr<CommandInterface>>> queue)
//    : _commands(std::move(queue)) {
//  BOOST_LOG_TRIVIAL(debug) << "Worker: create worker";
//}
//
//Worker::Worker(const Worker &worker)
//    : _commands(worker._commands) {
//  BOOST_LOG_TRIVIAL(debug) << "Worker: worker &";
//}
//
//Worker &Worker::operator=(const Worker &worker) {
//  BOOST_LOG_TRIVIAL(debug) << "Worker: worker operator=";
//  Worker copy(worker);
//  std::swap(*this, copy);
//  return *this;
//}
//
//Worker::Worker(const Worker &&worker)
//    : _commands(std::move(worker._commands)) {
//  BOOST_LOG_TRIVIAL(debug) << "Worker: worker &&";
//}

void Worker::Run(std::queue<std::shared_ptr<BaseCommand>> &queue) {
  while (!queue.empty()) {
    BOOST_LOG_TRIVIAL(info) << "Worker: new command";
    auto command = queue.front();
    queue.pop();
    command->Do();
  }
}