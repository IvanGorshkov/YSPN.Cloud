#pragma once

#include "Net.h"
#include "Commands.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>
#include <functional>
#include <memory>

namespace pt = boost::property_tree;

class Worker {
 public:
  explicit Worker(std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> get, bool &isWork);
  Worker(const Worker &worker);
//  Worker(const Worker &&worker);
  ~Worker();

  void Run();

 private:
  void listening();
  static void onConnect(const std::shared_ptr<std::pair<UserSession, pt::ptree>> &user);
  std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> getConnection;

 private:
  static std::unique_ptr<Command> getCommand(const pt::ptree &pt);
  bool &_isWork;
};
