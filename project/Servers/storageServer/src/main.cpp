#include "StorageServer.h"
#include "Config.h"
#include "Log.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message");

  po::options_description configDesc("Config options REQUIRED");
  configDesc.add_options()
      ("input,i", po::value<std::string>(), "Config file");

  po::options_description logDesc("Log options");
  logDesc.add_options()
      ("config,c", po::value<std::string>(), "System config {release, debug}")
      ("output,o", po::value<std::string>(), "Output log file");

  desc.add(configDesc).add(logDesc);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }

  if (!vm.count("input")) {
    std::cout << "Choice config file" << std::endl;
    return EXIT_FAILURE;
  }

  if (vm.count("config") && vm.count("output")) {
    init_log(vm["output"].as<std::string>(), vm["config"].as<std::string>());
  }

  auto &config = Config::GetInstance();
  if (config.Run(vm["input"].as<std::string>()) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  StorageServer server;

  int a;
  while (std::cin >> a) {
    switch (a) {
//      case -1: {
//        server.Stop();
//        break;
//      }

      case 1: {
        server.Run();
        break;
      }

      case 2: {
        server.put2();
        break;
      }

      case 0: {
        return EXIT_SUCCESS;
      }

      default: {
        server.put();
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
