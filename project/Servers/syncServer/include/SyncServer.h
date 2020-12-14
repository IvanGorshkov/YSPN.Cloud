#pragma once

#include <boost/property_tree/ptree.hpp>
#include <queue>
#include <utility>
#include <thread>
#include "Server.h"
#include "Worker.h"
#include "SyncManager.h"
#include "NetworkServer.h"
#include "ServerNetworkExceptions.h"
#include "Config.h"

namespace pt = boost::property_tree;

class SyncServer : public Server {
 public:
  explicit SyncServer();
  ~SyncServer() override;

  void Run() override;

 private:
  void startWorkers() override;
  void stopWorkers() override;

 private:
  std::vector<std::thread> _workerThreads;
  Config &_config;
  std::shared_ptr<NetworkServer> _networkServer;
};