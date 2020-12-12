#pragma once

#include <queue>
#include "Commands.h"

class Worker {
 public:
  Worker(const std::queue<std::shared_ptr<CommandInterface>> &queue);

  void Run();

 private:
  void listening();

 private:
  std::queue<std::shared_ptr<CommandInterface>> _commands;
};