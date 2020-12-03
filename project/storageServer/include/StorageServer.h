#pragma once

#include "Worker.h"
#include "Net.h"
#include "Config.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>
#include <queue>
#include <utility>
#include <thread>

namespace pt = boost::property_tree;

class StorageServer {
 public:
  explicit StorageServer();
  ~StorageServer();

  void Run();
  void Stop();

  void put() {
    int id = std::rand() % 100;
    UserSession user(id);
    pt::ptree root;

    root.put("command", "UploadChunk");
    pt::ptree data;
    for (int i = 0; i < 4; i++) {
      pt::ptree child;
      child.put("userId", id);
      child.put("chunkId", i + 1);
      child.put("sHash", "shash");
      child.put("rHash", "rhash");
      child.put("data", "data");

      data.push_back(std::make_pair("", child));
    }

    root.add_child("data", data);

    auto pair = std::make_pair(user, root);
    auto share = std::make_shared<std::pair<UserSession, pt::ptree>>(pair);
    networkServer.PutConnection(share);
  }

 private:
  void startWorkers();
  void stopWorkers();

 private:
  std::vector<std::thread> _workerThreads;
  bool _isWorkingWorker;
  Config &_config;
  NetworkServer networkServer;
};
