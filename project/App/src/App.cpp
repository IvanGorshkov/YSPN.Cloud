#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App(std::function<void(const std::string &msg)> callbackOk,
         std::function<void(const std::string &msg)> callbackError)
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")),
      appCallbackOk(std::move(callbackOk)),
      appCallbackError(std::move(callbackError)) {
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
  return _internalDB->IsExistUser();
}

void App::LoginUser(std::string login, const std::string& password) {
  BOOST_LOG_TRIVIAL(debug) << "App: LoginUser";

  auto loginUser = LoginUserCommand(appCallbackOk,
                                    appCallbackError,
                                    _internalDB,
                                    std::move(login),
                                    std::move(password));
  loginUser.Do();
}

void App::RegisterUser(std::string login, const std::string& password) {
  BOOST_LOG_TRIVIAL(debug) << "App: RegisterUser";

  auto registerUser = RegisterUserCommand(appCallbackOk,
                                          appCallbackError,
                                          _internalDB,
                                          std::move(login),
                                          std::move(password));
  registerUser.Do();
}

void App::Logout() {
  BOOST_LOG_TRIVIAL(debug) << "App: Logout";

  _internalDB->DeleteUser();
}

std::string App::hash(const std::string &password) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5(reinterpret_cast<const unsigned char *>(password.c_str()), password.size(), digest);

  std::stringstream result;
  for (unsigned char i : digest)
    result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

  return result.str();
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
  BOOST_LOG_TRIVIAL(debug) << "App: CreateFile";

  if (!fs::exists(path)) {
    throw FileNotExistsException("this file does not exist");
  }

  BOOST_LOG_TRIVIAL(info) << "App: create file " << path.string();

  if (!_watcher.IsWorking()) {
    createFile(path);
  }
}

void App::RenameFile(const fs::path &oldPath, const fs::path &newPath) {
  BOOST_LOG_TRIVIAL(debug) << "App: RenameFile";

  if (!fs::exists(newPath)) {
    throw FileNotExistsException("this file does not exist");
  }

  BOOST_LOG_TRIVIAL(info) << "App: rename file " << oldPath.string() << " to " << newPath.string();

  if (!_watcher.IsWorking()) {
    renameFile(oldPath, newPath);
  }
}

void App::DeleteFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: DeleteFile";

  BOOST_LOG_TRIVIAL(info) << "App: delete file " << path.string();

  if (!_watcher.IsWorking()) {
    deleteFile(path);
  }
}

void App::ModifyFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: ModifyFile";

  if (!fs::exists(path)) {
    throw FileNotExistsException("this file does not exist");
  }

  BOOST_LOG_TRIVIAL(info) << "App: modify file " << path.string();

  if (!_watcher.IsWorking()) {
    modifyFile(path);
  }
}

void App::createFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: createFile";

  auto sh = std::make_shared<CreateFileCommand>(appCallbackOk, appCallbackError, _internalDB, path);
  _commands.emplace(sh);

  runWorker();
}

void App::renameFile(const fs::path &oldPath, const fs::path &newPath) {
  BOOST_LOG_TRIVIAL(debug) << "App: renameFile";

  auto sh = std::make_shared<RenameFileCommand>(appCallbackOk, appCallbackError, _internalDB, oldPath, newPath);
  _commands.emplace(sh);

  runWorker();
}

void App::deleteFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: deleteFile";

  auto sh = std::make_shared<DeleteFileCommand>(appCallbackOk, appCallbackError, _internalDB, path);
  _commands.emplace(sh);

  runWorker();
}

void App::modifyFile(const fs::path &path) {
  BOOST_LOG_TRIVIAL(debug) << "App: modifyFile";

  auto sh = std::make_shared<ModifyFileCommand>(appCallbackOk, appCallbackError, _internalDB, path);
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
  BOOST_LOG_TRIVIAL(info) << "App: execEvent";

  auto event = _events.front();
  _events.pop();

  switch (event.event) {
    case CREATE: {
      BOOST_LOG_TRIVIAL(info) << "App: createFile";
      createFile(event.path);
      break;
    }

    case RENAME: {
      BOOST_LOG_TRIVIAL(info) << "App: renameFile";
      renameFile(event.path, event.new_path.value());
      break;
    }

    case MODIFY: {
      BOOST_LOG_TRIVIAL(info) << "App: modifyFile";
      modifyFile(event.path);
      break;
    }

    case DELETE: {
      BOOST_LOG_TRIVIAL(info) << "App: deleteFile";
      deleteFile(event.path);
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

  BOOST_LOG_TRIVIAL(info) << "App: new event " << event.event;
  _events.push(event);

  execEvent();
}
