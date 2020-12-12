#pragma once

#include <vector>
#include <queue>
#include <functional>
#include "Commands.h"

class App {
 public:
  App();
  void Refresh(const std::function<void(const std::string &msg)> &callback);

  std::vector<int> getFiles();
  void downloadFile(int fileId, const std::function<void(const std::string &msg)> &callback);

  std::vector<int> getEvents();
  void SaveEvents(const std::function<void(const std::string &msg)> &callback);

 private:
  void refresh();
  std::vector<int> events;
  std::queue<std::shared_ptr<CommandInterface>> commands;
  // shared Internal DB
  // shared meta network
  // shared cloud network
};