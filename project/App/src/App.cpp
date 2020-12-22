#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App(const std::function<void()> &callbackOk,
         const std::function<void(const std::string &msg)> &callbackError)
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")),
      ok(callbackOk),
      ne0k(callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: create app";
//  ClientConfig::Log("release");

  runWatcher();
}

App::~App() {
  BOOST_LOG_TRIVIAL(debug) << "App: delete app";
  stopWatcher();
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

void App::UploadFile(const fs::path &path) {

  BOOST_LOG_TRIVIAL(debug) << "App: UploadFile";

  if (!fs::exists(path)) {
    throw FileNotExistsException("this file does not exist");
  }

  auto sh = std::make_shared<FileCommand>(ok, ne0k, _internalDB, path);
  _commands.emplace(sh);

  runWorker();
}

void App::RenameFile(const fs::path &oldPath, const fs::path &newPath) {
  BOOST_LOG_TRIVIAL(debug) << "App: RenameFile";

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<FileCommand>(ok, ne0k, _internalDB, oldPath, newPath);
    _commands.emplace(sh);

    runWorker();
  }
}

void App::DeleteFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: DeleteFile";

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<FileCommand>(ok, ne0k, _internalDB, path, boost::none, true);
    _commands.emplace(sh);

    runWorker();
  }
}

void App::ModifyFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: ModifyFile";

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<FileCommand>(ok, ne0k, _internalDB, path);
    _commands.emplace(sh);

    runWorker();
  }
}

void App::UpdateSyncFolder(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: UpdateSyncFolder";
  _watcher.Stop();
  _watcherThread.join();
  if (!fs::exists(path)) {
    throw FolderNotExistsException("this folder does not exist");
  }
  stopWatcher();

  _internalDB->UpdateSyncFolder(path.string());

  runWatcher();
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

void App::runWatcher() {
  BOOST_LOG_TRIVIAL(debug) << "App: runWatcher";

  _watcherThread = std::thread([&]() {
    _watcher.Run(_internalDB->GetSyncFolder(),
                 std::bind(&App::watcherCallback, this, std::placeholders::_1));
  });
}

void App::stopWatcher() {
  BOOST_LOG_TRIVIAL(debug) << "App: stopWatcher";

  _watcher.Stop();
  _watcherThread.join();
}

void App::execEvent() {
  BOOST_LOG_TRIVIAL(debug) << "App: execEvent";

  auto event = _events.front();
  _events.pop();

  switch (event.event) {
    case CREATE: {
      UploadFile(event.path);
      break;
    }

    case RENAME: {
      RenameFile(event.path, event.new_path.value());
      break;
    }

    case MODIFY: {
      ModifyFile(event.path);
      break;
    }

    case DELETE: {
      DeleteFile(event.path);
      break;
    }

    default: {
      BOOST_LOG_TRIVIAL(error) << "App: execEvent error event";
      break;
    }
  }
}

void App::watcherCallback(CloudNotification event) {
  BOOST_LOG_TRIVIAL(debug) << "App: watcherCallback";

  std::cout << event.event << std::endl;

  _events.push(event);
}


