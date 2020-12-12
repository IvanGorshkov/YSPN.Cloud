#include "ClientApp.h"
#include <iostream>

void ClientApp::Watch() {
  int command;
  do {
    printCommandList();

    std::cin >> command;
  } while (parseCommand(command) != EXIT_FAILURE);
}

void ClientApp::printCommandList() {
  std::cout << "Command List:" << std::endl;
  std::cout << "0) Exit" << std::endl;
  std::cout << "1) Refresh" << std::endl;
  std::cout << "2) ShowFiles" << std::endl;
  std::cout << "3) DownloadFile" << std::endl;
  std::cout << "4) ShowEvents" << std::endl;
  std::cout << "5) SaveEvents" << std::endl;
}

int ClientApp::parseCommand(int command) {
  switch (command) {
    case 0: {
      return EXIT_FAILURE;
    }

    case 1: {
      refresh();
      return EXIT_SUCCESS;
    }

    case 2: {
      showFiles();
      return EXIT_SUCCESS;
    }

    case 3: {
      downloadFile();
      return EXIT_SUCCESS;
    }

    case 4: {
      showEvents();
      return EXIT_SUCCESS;
    }

    case 5: {
      saveEvents();
      return EXIT_SUCCESS;
    }

    default: {
      std::cout << "Wrong command" << std::endl << std::endl;
      return EXIT_SUCCESS;
    }
  }
}

void ClientApp::refresh() {
  std::cout << "Refresh: " << std::endl;
  app.Refresh(&ClientApp::refreshCallback);
}

void ClientApp::refreshCallback(const std::string &msg) {
  std::cout << "refreshCallback " << msg << std::endl;

  // TODO refreshCallback print
}

void ClientApp::showFiles() {
  std::cout << "Files: " << std::endl;

  auto files = app.getFiles();

  // TODO print files
}

void ClientApp::downloadFile() {
  showFiles();

  std::cout << "Choice file: ";

  int file;
  std::cin >> file;

  app.downloadFile(file, &ClientApp::downloadFileCallback);
}

void ClientApp::downloadFileCallback(const std::string &msg) {
  std::cout << "downloadFileCallback " << msg << std::endl;

  // TODO downloadFileCallback print
}

void ClientApp::showEvents() {
  std::cout << "Events: " << std::endl;

  auto events = app.getEvents();

  // TODO print events
}

void ClientApp::saveEvents() {
  app.SaveEvents(&ClientApp::saveEventsCallback);

  // TODO check that is all
}

void ClientApp::saveEventsCallback(const std::string &msg) {
  std::cout << "saveEventsCallback " << msg << std::endl;

  // TODO saveEventsCallback print
}
