#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App(const std::function<void(const std::string &msg)> &callbackOk,
         const std::function<void(const std::string &msg)> &callbackError)
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")),
      appCallbackOk(callbackOk),
      appCallbackError(callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: create app";
//  ClientConfig::Log("release");

  runWatcher();
}

App::~App() {
  BOOST_LOG_TRIVIAL(debug) << "App: delete app";
  stopWatcher();
}

bool App::IsLogin() const {
  BOOST_LOG_TRIVIAL(debug) << "App: IsLogin";
  return true;
}

void App::Refresh() {
  BOOST_LOG_TRIVIAL(debug) << "App: Refresh";

  auto sh = std::make_shared<RefreshCommand>(appCallbackOk, appCallbackError, _internalDB);
  _commands.emplace(sh);

  runWorker();
}

std::vector<FileMeta> App::GetFiles() {
  BOOST_LOG_TRIVIAL(debug) << "App: GetFiles";
  return _internalDB->SelectAllFiles();
}

void App::DownloadFile(int fileId) {
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

  BOOST_LOG_TRIVIAL(info) << "App: download file " << file.filePath + file.fileName + file.fileExtension;

  auto sh = std::make_shared<DownloadFileCommand>(appCallbackOk, appCallbackError, _internalDB, file);
  _commands.emplace(sh);

  runWorker();
}

void App::CreateFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: UploadFile";

  if (!fs::exists(path)) {
    throw FileNotExistsException("this file does not exist");
  }

  BOOST_LOG_TRIVIAL(info) << "App: upload file " << path.string();

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<CreateFileCommand>(appCallbackOk, appCallbackError, _internalDB, path);
    _commands.emplace(sh);

    runWorker();
  }
}

void App::RenameFile(const fs::path &oldPath, const fs::path &newPath) {
  BOOST_LOG_TRIVIAL(debug) << "App: RenameFile";

  if (!fs::exists(newPath)) {
    throw FileNotExistsException("this file does not exist");
  }

  BOOST_LOG_TRIVIAL(info) << "App: rename file " << oldPath.string() << " to " << newPath.string();

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<RenameFileCommand>(appCallbackOk, appCallbackError, _internalDB, oldPath, newPath);
    _commands.emplace(sh);

    runWorker();
  }
}

void App::DeleteFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: DeleteFile";

  BOOST_LOG_TRIVIAL(info) << "App: delete file " << path.string();

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<DeleteFileCommand>(appCallbackOk, appCallbackError, _internalDB, path);
    _commands.emplace(sh);

    runWorker();
  }
}

void App::ModifyFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: ModifyFile";

  if (!fs::exists(path)) {
    throw FileNotExistsException("this file does not exist");
  }

  BOOST_LOG_TRIVIAL(info) << "App: modify file " << path.string();

  if (!_watcher.IsWorking()) {
    auto sh = std::make_shared<ModifyFileCommand>(appCallbackOk, appCallbackError, _internalDB, path);
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

  BOOST_LOG_TRIVIAL(info) << "App: old sync folder " << _internalDB->GetSyncFolder();

  stopWatcher();
  _internalDB->UpdateSyncFolder(path.string());
  runWatcher();

  BOOST_LOG_TRIVIAL(info) << "App: new sync folder " << _internalDB->GetSyncFolder();
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
      CreateFile(event.path);
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

void App::watcherCallback(const CloudNotification &event) {
  BOOST_LOG_TRIVIAL(debug) << "App: watcherCallback";

  BOOST_LOG_TRIVIAL(info) << "App: new event" << event.event;
  _events.push(event);

  execEvent();
}
