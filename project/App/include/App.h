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

  void UploadFile(const fs::path &path);
  void RenameFile(const fs::path &oldPath, const fs::path &newPath);
  void DeleteFile(const fs::path &path);
  void ModifyFile(const fs::path &path);

  void UpdateSyncFolder(const fs::path &path);
  std::string GetSyncFolder();

 private:
  void runWorker();

  void runWatcher();
  void stopWatcher();
  void execEvent();
  void watcherCallback(CloudNotification event);

 private:
  std::queue<std::shared_ptr<BaseCommand>> _commands;
  std::shared_ptr<InternalDB> _internalDB;

  std::queue<CloudNotification> _events;
  std::thread _watcherThread;
  Watcher _watcher;
};