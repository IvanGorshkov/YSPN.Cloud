#pragma once

#include <boost/property_tree/ptree.hpp>
#include <functional>
#include <memory>
#include "NetworkSever.h"
#include "CommandManager.h"

namespace pt = boost::property_tree;

class Worker {
 public:
  explicit Worker(std::shared_ptr<CommandManager> manager,
                  std::shared_ptr<NetworkSever> server);
  ~Worker();

  void Run();

 private:
  void listening();
  void onConnect(const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, pt::ptree>> &request);

 private:
  std::shared_ptr<NetworkSever> _networkServer;
  std::shared_ptr<CommandManager> _manager;
};
