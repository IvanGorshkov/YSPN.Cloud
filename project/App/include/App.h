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
  App(const std::function<void(const std::string &msg)> &callbackOk,
      const std::function<void(const std::string &msg)> &callbackError);
  ~App();

  bool IsLogin() const;

  void Refresh();

  std::vector<FileMeta> GetFiles();
  void DownloadFile(int fileId) noexcept(false);

  void CreateFile(const fs::path &path) noexcept(false);
  void RenameFile(const fs::path &oldPath, const fs::path &newPath) noexcept(false);
  void DeleteFile(const fs::path &path);
  void ModifyFile(const fs::path &path) noexcept(false);

  void UpdateSyncFolder(const fs::path &path) noexcept(false);
  std::string GetSyncFolder();

 private:
  void runWorker();

  void runWatcher();
  void stopWatcher();
  void execEvent();
  void watcherCallback(const CloudNotification &event);

 private:
  std::function<void(const std::string &msg)> appCallbackOk;
  std::function<void(const std::string &msg)> appCallbackError;

 private:
  std::queue<std::shared_ptr<BaseCommand>> _commands;
  std::shared_ptr<InternalDB> _internalDB;

  std::queue<CloudNotification> _events;
  std::thread _watcherThread;
  Watcher _watcher;
};