#pragma once

#include <boost/property_tree/ptree.hpp>
#include <queue>
#include <utility>
#include <thread>
#include "Server.h"
#include "Worker.h"
#include "StorageManager.h"
#include "Net.h"
#include "Config.h"

namespace pt = boost::property_tree;

class StorageServer : public Server {
 public:
  explicit StorageServer();
  ~StorageServer() override;

  void Run() override;

  void put() {
    int id = 2;
    UserSession user(id);
    pt::ptree root;

    root.put("command", "UploadChunk");
    root.put("requestId", std::rand() % 10);
    pt::ptree data;
    for (int i = 0; i < 2; i++) {
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

  void put2() {
    int id = 2;
    UserSession user(id);
    pt::ptree root;

    root.put("command", "DownloadChunk");
    root.put("requestId", std::rand() % 10);
    pt::ptree data;
    for (int i = 0; i < 2; i++) {
      pt::ptree child;
      child.put("userId", id);
      child.put("chunkId", i + 1);

      data.push_back(std::make_pair("", child));
    }

    root.add_child("data", data);

    auto pair = std::make_pair(user, root);
    auto share = std::make_shared<std::pair<UserSession, pt::ptree>>(pair);
    networkServer.PutConnection(share);
  }

 private:
  void startWorkers() override;
  void stopWorkers() override;

 private:
  std::vector<std::thread> _workerThreads;
  Config &_config;
  NetworkServer networkServer;
};
