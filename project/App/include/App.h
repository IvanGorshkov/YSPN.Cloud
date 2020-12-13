#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include "ClientNetwork.h"
#include "Commands.h"
#include "InternalDB.h"
#include "Worker.h"
#include "AppExceptions.h"
#include "InternalExceptions.h"

class App {
 public:
  App();
  ~App();
  void Refresh(const std::function<void(const std::string &msg)> &callback);

  std::vector<Files> getFiles();
  void downloadFile(int fileId,
                    const std::function<void(const std::string &msg)> &callbackOk,
                    const std::function<void(const std::string &msg)> &callbackError) noexcept(false);

  std::vector<int> getEvents();
  void SaveEvents(const std::function<void(const std::string &msg)> &callback);

 private:
  void refresh();
  std::queue<int> _events;
  std::queue<std::shared_ptr<CommandInterface>> _commands;

  bool _isWorkingWorker;
  std::thread _worker;

  std::shared_ptr<InternalDB> _internalDB;
  std::shared_ptr<ClientNetwork> _storageNetwork;
  // shared meta network
  // shared cloud network
};