#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include "Commands.h"
#include "InternalDB.h"
#include "Worker.h"
#include "AppExceptions.h"
#include "InternalExceptions.h"

class App {
 public:
  App();
  ~App();
  void Refresh(const std::function<void()> &callbackOk,
               const std::function<void(const std::string &msg)> &callbackError);

  std::vector<Files> GetFiles();
  void DownloadFile(int fileId,
                    const std::function<void()> &callbackOk,
                    const std::function<void(const std::string &msg)> &callbackError) noexcept(false);

  std::vector<int> GetEvents();
  void SaveEvents(const std::function<void()> &callbackOk,
                  const std::function<void(const std::string &msg)> &callbackError);

 private:
  void runWorker();

 private:
  std::queue<int> _events;
  std::queue<std::shared_ptr<BaseCommand>> _commands;
  std::shared_ptr<InternalDB> _internalDB;
};