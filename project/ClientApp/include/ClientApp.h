#pragma once

#include "../../App/include/App.h"

class ClientApp {
 public:
  void Watch();

 private:
  static void printCommandList();
  int parseCommand(int command);

  void refresh();
  static void refreshCallback(const std::string &msg);

  void showFiles();
  void downloadFile();
  static void downloadFileCallback(const std::string &msg);

  void showEvents();
  void saveEvents();
  static void saveEventsCallback(const std::string &msg);

 private:
  App app;
};