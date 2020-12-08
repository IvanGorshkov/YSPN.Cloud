#pragma once

#include <boost/property_tree/ptree.hpp>
#include "Command.h"

namespace pt = boost::property_tree;

class CommandManager {
 public:
  virtual ~CommandManager() = default;
  virtual std::shared_ptr<pt::ptree> GetRequest(pt::ptree &val) = 0;

 private:
  virtual std::unique_ptr<Command> getCommand(const pt::ptree &val) = 0;
};