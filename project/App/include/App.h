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
#include "structs/CloudEvents.h"
#include "Watcher.h"
#include "Notification.h"

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

  std::vector<Notification> GetEvents();
  void execEvents();

  void UploadFile(const fs::path &path,
                  const std::function<void()> &callbackOk,
                  const std::function<void(const std::string &msg)> &callbackError);

  void UpdateSyncFolder(const fs::path &path);
  std::string GetSyncFolder();

 private:
  void runWorker();
  void callBack(CloudNotification);

 private:
  std::queue<CloudNotification> _events;
  std::queue<std::shared_ptr<BaseCommand>> _commands;
  std::shared_ptr<InternalDB> _internalDB;

  std::thread _watcherThread;
  Watcher _watcher;
};