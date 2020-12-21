#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App()
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")) {
  BOOST_LOG_TRIVIAL(debug) << "App: create app";
//  ClientConfig::Log("release");
  _watcherThread = std::thread([&]() { _watcher.Run(_internalDB->GetSyncFolder(), std::bind(&App::callBack, this, std::placeholders::_1)); });
std::cout<<std::getenv("HOME");
}

App::~App() {
  BOOST_LOG_TRIVIAL(debug) << "App: delete app";
  _watcherThread.join();
  }


void App::Refresh(const std::function<void()> &callbackOk,
                  const std::function<void(const std::string &msg)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: Refresh";

  auto sh = std::make_shared<RefreshCommand>(callbackOk, callbackError, _internalDB);
  _commands.emplace(sh);

  runWorker();
}

std::vector<FileMeta> App::GetFiles() {
  BOOST_LOG_TRIVIAL(debug) << "App: GetFiles";
  return _internalDB->SelectAllFiles();
}

void App::DownloadFile(int fileId,
                       const std::function<void()> &callbackOk,
                       const std::function<void(const std::string &msg)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: DownloadFile";

  FileMeta file;
  try {
    file = _internalDB->SelectFile(fileId);
  } catch (InternalExceptions &er) {
    throw FileIdException(er.what());
  }

  if (file.isDownload) {
    throw FileDownloadedException("file is already downloaded");
  }

  auto sh = std::make_shared<DownloadFileCommand>(callbackOk, callbackError, _internalDB, file);
  _commands.emplace(sh);

  runWorker();
}

void App::callBack(CloudNotification new_event){
  _events.push(new_event);
  std::cout<<new_event.event<<std::endl;
}

//std::vector<Notification> App::GetEvents() {
//  BOOST_LOG_TRIVIAL(debug) << "App: GetEvents";
//  return _events;
//}

void App::execEvents() {
  BOOST_LOG_TRIVIAL(debug) << "App: SaveEvents";
  // TODO SaveEvents loop create event command and send to queue
  CloudNotification new_notif = _events.front();
  _events.pop();
}

void App::UploadFile(const fs::path &path,
                     const std::function<void()> &callbackOk,
                     const std::function<void(const std::string &)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: UploadFile";

  if (!fs::exists(path)) {
    throw FileNotExistsException("this file does not exist");
  }

  auto sh = std::make_shared<FileCommand>(callbackOk, callbackError, _internalDB, path);
  _commands.emplace(sh);

  runWorker();
}

void App::UpdateSyncFolder(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: UpdateSyncFolder";
  _watcher.Stop();
  _watcherThread.join();
  if (!fs::exists(path)) {
    throw FolderNotExistsException("this folder does not exist");
  }
  _internalDB->UpdateSyncFolder(path.string());
  _watcherThread = std::thread([&]() { _watcher.Run(_internalDB->GetSyncFolder(), std::bind(&App::callBack, this, std::placeholders::_1)); });

}

std::string App::GetSyncFolder() {
  BOOST_LOG_TRIVIAL(debug) << "App: GetSyncFolder";

  return _internalDB->GetSyncFolder();
}

void App::runWorker() {
  BOOST_LOG_TRIVIAL(debug) << "App: runWorker";

  auto worker = std::thread(&Worker::Run, std::ref(_commands));
  worker.detach();
}


