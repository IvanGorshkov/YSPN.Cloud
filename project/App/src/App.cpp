#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App()
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")) {
  BOOST_LOG_TRIVIAL(debug) << "App: create app";
//  ClientConfig::Log("release");
}

App::~App() {
  BOOST_LOG_TRIVIAL(debug) << "App: delete app";
}

void App::Refresh(const std::function<void()> &callbackOk,
                  const std::function<void(const std::string &msg)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: Refresh";

  auto sh = std::make_shared<RefreshCommand>(callbackOk, callbackError, _internalDB);
  _commands.emplace(sh);

  runWorker();
}

std::vector<Files> App::GetFiles() {
  BOOST_LOG_TRIVIAL(debug) << "App: GetFiles";
  return _internalDB->SelectAllFiles();
}

void App::DownloadFile(int fileId,
                       const std::function<void()> &callbackOk,
                       const std::function<void(const std::string &msg)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: DownloadFile";

  Files file;
  try {
    file = _internalDB->SelectFile(fileId);
  } catch (InternalExceptions &er) {
    throw FileIdException(er.what());
  }

  if (file.is_download.value()) {
    throw FileDownloadedException("file is already downloaded");
  }

  auto sh = std::make_shared<DownloadFileCommand>(callbackOk, callbackError, _internalDB, file);
  _commands.emplace(sh);

  runWorker();
}

std::vector<int> App::GetEvents() {
  BOOST_LOG_TRIVIAL(debug) << "App: GetEvents";
  // TODO getEvents return events queue
}

void App::SaveEvents(const std::function<void()> &callbackOk,
                     const std::function<void(const std::string &msg)> &callbackError) {
  BOOST_LOG_TRIVIAL(debug) << "App: SaveEvents";
  // TODO SaveEvents loop create event command and send to queue

  callbackError("test");
}

void App::runWorker() {
  BOOST_LOG_TRIVIAL(debug) << "App: runWorker";

  auto worker = std::thread(&Worker::Run, std::ref(_commands));
  worker.detach();
}


