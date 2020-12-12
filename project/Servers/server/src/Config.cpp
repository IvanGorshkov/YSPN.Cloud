#include "Config.h"
#include <iomanip>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

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
    parseLogConfig(root.get_child("log"));
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

void Config::parseLogConfig(pt::ptree &log) {
  BOOST_LOG_TRIVIAL(debug) << "Config: parseLogConfig";
  if (log.get<std::string>("state") == "release") {
    logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );
  }

  if (!log.get<std::string>("file").empty()) {
    logging::add_file_log
        (
            keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation =
                sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format =
                (
                    expr::stream
                        << std::hex << std::setw(8) << std::setfill('0') << expr::attr<unsigned int>("LineID")
                        << "\t"
                        << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f")
                        << "\t:"
                        << " <" << logging::trivial::severity << "> "
                        << "\t"
                        << expr::smessage
                )
        );
  }
}

void Config::parseStorageConfig(pt::ptree &storage) {
  BOOST_LOG_TRIVIAL(debug) << "Config: parseStorageConfig";
  _storageConfig.workersCount = storage.get<size_t>("workersCount");
}

void Config::parseNetworkConfig(boost::property_tree::ptree &network) {
  BOOST_LOG_TRIVIAL(debug) << "Config: parseNetworkConfig";
}
