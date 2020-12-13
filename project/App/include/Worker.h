#pragma once

#include <queue>
#include "Commands.h"

class Worker {
 public:
  static void Run(std::queue<std::shared_ptr<CommandInterface>> &queue, bool &isWorking);
};