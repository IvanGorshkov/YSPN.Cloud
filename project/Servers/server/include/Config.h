#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

struct StorageConfig {
  size_t workersCount;
};

struct NetworkConfig {
  short port;
  short backlog;
};

struct ClientWorkerConfig {
  size_t requestWorkersCount;
  size_t responceWorkersCount;
};

class Config {
 public:
  int Run(const std::string &path);
  StorageConfig &GetStorageConfig();
  NetworkConfig &GetNetworkConfig();
  ClientWorkerConfig &GetClientWorkerConfig();
  static Config &GetInstance();

 private:
  Config();
  ~Config();
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  static void parseLogConfig(pt::ptree &log);
  void parseStorageConfig(pt::ptree &storage);
  void parseNetworkConfig(pt::ptree &network);
  void parseClientWorkerConfig(pt::ptree &clientWorker);

 private:
  StorageConfig _storageConfig{};
  NetworkConfig _networkConfig{};
  ClientWorkerConfig _clientWorkerConfig{};
};
