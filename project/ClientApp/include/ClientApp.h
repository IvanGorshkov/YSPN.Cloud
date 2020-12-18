#pragma once

#include "../../App/include/App.h"

class ClientApp {
 public:
  void Watch();

 private:
  static void printCommandList();
  int parseCommand(int command);

  void refresh();
  static void refreshCallbackOk();
  static void refreshCallbackError(const std::string &msg);

  void showFiles();
  void downloadFile();
  static void downloadFileCallbackOk();
  static void downloadFileCallbackError(const std::string &msg);

  void showEvents();
  void saveEvents();
  static void saveEventsCallbackOk();
  static void saveEventsCallbackError(const std::string &msg);

  void uploadFile();
  static void uploadFileCallbackOk();
  static void uploadFileCallbackError(const std::string &msg);

  void changeDefaultFolder();

 private:
  App app;
};