#pragma once

#include <boost/property_tree/ptree.hpp>
#include <functional>
#include <memory>
#include "Net.h"
#include "CommandManager.h"

namespace pt = boost::property_tree;

class Worker {
 public:
  explicit Worker(std::shared_ptr<CommandManager> manager,
                  std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> get);
  ~Worker();

  void Run();

 private:
  void listening();
  void onConnect(const std::shared_ptr<std::pair<UserSession, pt::ptree>> &user);

 private:
  std::function<std::shared_ptr<std::pair<UserSession, pt::ptree>>()> getConnection;
  std::shared_ptr<CommandManager> _manager;
};
