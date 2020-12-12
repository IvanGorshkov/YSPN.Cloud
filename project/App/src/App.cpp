#include "App.h"
#include <string>

App::App() {
  refresh();
}

void App::Refresh(const std::function<void(const std::string &msg)> &callback) {
  refresh();
  callback("dsdsd");
}

std::vector<int> App::getFiles() {
  // TODO Ivan GetFiles from Internal DB

  return std::vector<int>();
}

void App::downloadFile(int fileId, const std::function<void(const std::string &msg)> &callback) {
  // TODO create downloadFileCommand and sent to queue

  callback("dsdsd");
}

std::vector<int> App::getEvents() {
  return events;
}

void App::SaveEvents(const std::function<void(const std::string &msg)> &callback) {
  // TODO loop create event command and send to queue

  callback("dsdsd");
}

void App::refresh() {
  // TODO create refreshCommand and send to queue
}


