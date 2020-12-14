#pragma once

#include <boost/property_tree/ptree.hpp>
#include "CommandManager.h"
#include "SyncExceptions.h"
#include "SerializerAnswer.h"
#include "SyncCommands.h"

namespace pt = boost::property_tree;

class SyncManager : public CommandManager {
 public:
  std::shared_ptr<pt::ptree> GetRequest(pt::ptree &val) override;

 private:
  std::unique_ptr<Command> getCommand(const pt::ptree &val) override;
};