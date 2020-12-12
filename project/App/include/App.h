#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include "Commands.h"
#include "InternalDB.h"
#include "Worker.h"
#include "AppExceptions.h"

class App {
 public:
  App();
  ~App();
  void Refresh(const std::function<void(const std::string &msg)> &callback);

  std::vector<Files> getFiles();
  void downloadFile(int fileId, const std::function<void(const std::string &msg)> &callback) noexcept(false);

  std::vector<int> getEvents();
  void SaveEvents(const std::function<void(const std::string &msg)> &callback);

 private:
  void refresh();
  std::vector<int> _events;
  std::queue<std::shared_ptr<CommandInterface>> _commands;
  std::shared_ptr<InternalDB> _internalDB;
  std::thread _worker;
  // shared meta network
  // shared cloud network
};