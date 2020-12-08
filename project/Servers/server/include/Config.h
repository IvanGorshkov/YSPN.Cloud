#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

struct StorageConfig {
  size_t workersCount;
};

struct NetworkConfig {

};

class Config {
 public:
  int Run(const std::string &path);
  StorageConfig &GetStorageConfig();
  static Config &GetInstance();

 private:
  Config();
  ~Config();
  Config(const Config &);
  Config &operator=(const Config &);

  void parseStorageConfig(pt::ptree &storage);
  void parseNetworkConfig(pt::ptree &network);

 private:
  StorageConfig _storageConfig{};
  NetworkConfig _networkConfig{};
};
