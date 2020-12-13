#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App()
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")),
      _storageNetwork(std::make_shared<ClientNetwork>()),
      _isWorkingWorker(true),
      _worker(&Worker::Run, std::ref(_commands), std::ref(_isWorkingWorker)) {
  BOOST_LOG_TRIVIAL(debug) << "App: create app";
  ClientConfig::Log();
  _storageNetwork->Connect();
}

App::~App() {
  BOOST_LOG_TRIVIAL(debug) << "App: delete app";
  _isWorkingWorker = false;
  _worker.join();
}

void App::Refresh(const std::function<void(const std::string &msg)> &callback) {
  BOOST_LOG_TRIVIAL(debug) << "App: Refresh";
  refresh();
  callback("dsdsd");
}

std::vector<Files> App::getFiles() {
  BOOST_LOG_TRIVIAL(debug) << "App: getFiles";
  return _internalDB->SelectAllFiles();
}

void App::downloadFile(int fileId,
                       const std::function<void(const std::string &msg)> &callbackOk,
                       const std::function<void(const std::string &msg)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: downloadFile";

  Files file;
  try {
    file = _internalDB->SelectFile(fileId);
  } catch (InternalExceptions &er) {
    throw FileIdException(er.what());
  }

  if (file.is_download.value()) {
    throw FileDownloadedException("file is already downloaded");
  }

  auto sh = std::make_shared<DownloadFileCommand>(callbackOk, callbackError, _storageNetwork, file);
  _commands.emplace(sh);
}

std::vector<int> App::getEvents() {
  BOOST_LOG_TRIVIAL(debug) << "App: getEvents";

//  return _events;
}

void App::SaveEvents(const std::function<void(const std::string &msg)> &callback) {
  BOOST_LOG_TRIVIAL(debug) << "App: SaveEvents";
  // TODO loop create event command and send to queue

  callback("dsdsd");
}

void App::refresh() {
  BOOST_LOG_TRIVIAL(debug) << "App: refresh";
  // TODO create refreshCommand and send to queue
  auto sh = std::make_shared<RefreshCommand>(_internalDB);
  _commands.emplace(sh);
}


