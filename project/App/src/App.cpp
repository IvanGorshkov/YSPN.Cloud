#include "App.h"
#include "ClientConfig.h"
#include <string>
#include <boost/log/trivial.hpp>

App::App()
    : _internalDB(std::make_shared<InternalDB>("myDB.sqlite")),
      _worker(&Worker::Run, Worker(_commands)) {
  BOOST_LOG_TRIVIAL(debug) << "App: create app";
  ClientConfig::Log();
  refresh();
}

App::~App() {
  BOOST_LOG_TRIVIAL(debug) << "App: delete app";
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

void App::downloadFile(int fileId, const std::function<void(const std::string &msg)> &callback) {
  BOOST_LOG_TRIVIAL(debug) << "App: downloadFile";
  // TODO create downloadFileCommand and sent to queue

  auto file = _internalDB->SelectFile(fileId);

  callback("dsdsd");
}

std::vector<int> App::getEvents() {
  BOOST_LOG_TRIVIAL(debug) << "App: getEvents";
  return _events;
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


