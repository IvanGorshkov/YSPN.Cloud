#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <boost/filesystem.hpp>
#include "Commands.h"
#include "InternalDB.h"
#include "Worker.h"
#include "AppExceptions.h"
#include "InternalExceptions.h"

namespace fs = boost::filesystem;

class App {
 public:
  App();
  ~App();
  void Refresh(const std::function<void()> &callbackOk,
               const std::function<void(const std::string &msg)> &callbackError);

  std::vector<FileMeta> GetFiles();
  void DownloadFile(int fileId,
                    const std::function<void()> &callbackOk,
                    const std::function<void(const std::string &msg)> &callbackError) noexcept(false);

  std::vector<int> GetEvents();
  void SaveEvents(const std::function<void()> &callbackOk,
                  const std::function<void(const std::string &msg)> &callbackError);

  void UploadFile(const fs::path &path,
                  const std::function<void()> &callbackOk,
                  const std::function<void(const std::string &msg)> &callbackError);

  void UpdateSyncFolder(const fs::path &path);
  std::string GetSyncFolder();

 private:
  void runWorker();

 private:
  std::queue<int> _events;
  std::queue<std::shared_ptr<BaseCommand>> _commands;
  std::shared_ptr<InternalDB> _internalDB;
};