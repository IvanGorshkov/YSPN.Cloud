#include "Config.h"
#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>

Config::Config() {
  BOOST_LOG_TRIVIAL(debug) << "Config: create config";
}

Config::~Config() {
  BOOST_LOG_TRIVIAL(debug) << "Config: delete config";
}

Config &Config::GetInstance() {
  BOOST_LOG_TRIVIAL(debug) << "Config: GetInstance";
  static Config instance;
  return instance;
}

StorageConfig &Config::GetStorageConfig() {
  BOOST_LOG_TRIVIAL(debug) << "Config: GetStorageConfig";
  return _storageConfig;
}

int Config::Run(const std::string &path) {
  BOOST_LOG_TRIVIAL(debug) << "Config: Run";
  pt::ptree root;

  try {
    pt::read_json(path, root);
  } catch (pt::ptree_error &er) {
    BOOST_LOG_TRIVIAL(fatal) << "Config: " << er.what();
    return EXIT_FAILURE;
  }

  try {
    parseStorageConfig(root.get_child("storageConfig"));
  } catch (pt::ptree_error &er) {
    BOOST_LOG_TRIVIAL(fatal) << "Config: " << er.what();
    return EXIT_FAILURE;
  }

//  try {
//    parseNetworkConfig(root.get_child("networkConfig"));
//  } catch (pt::ptree_error &er) {
//    BOOST_LOG_TRIVIAL(fatal) << "Config: " << er.what();
//    return EXIT_FAILURE;
//  }

  return EXIT_SUCCESS;
}

void Config::parseStorageConfig(pt::ptree &storage) {
  BOOST_LOG_TRIVIAL(debug) << "Config: parseStorageConfig";
  _storageConfig.workersCount = storage.get<size_t>("workersCount");
}

void Config::parseNetworkConfig(boost::property_tree::ptree &network) {
  BOOST_LOG_TRIVIAL(debug) << "Config: parseNetworkConfig";
}
