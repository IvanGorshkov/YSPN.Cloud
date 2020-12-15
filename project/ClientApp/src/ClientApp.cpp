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
  std::cout << "6) UploadFile" << std::endl;
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

    case 6: {
      uploadFile();
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
  app.Refresh(&ClientApp::refreshCallbackOk, &refreshCallbackError);
}

void ClientApp::refreshCallbackOk() {
  std::cout << "refreshCallbackOk: " << "update complete" << std::endl;
}

void ClientApp::refreshCallbackError(const std::string &msg) {
  std::cout << "refreshCallbackError: " << msg << std::endl;
}

void ClientApp::showFiles() {
  std::cout << "Files: " << std::endl;

  auto files = app.GetFiles();

  for (auto &&file: files) {
    std::cout << file.id << ")";
    std::cout << " file: " << file.file_name << '.' << file.file_extention;
    std::cout << " version: " << file.version;
    std::cout << " size: " << file.file_size;
    std::cout << " download: " << file.is_download.value();
    std::cout << " update date: " << file.update_date;
    std::cout << " create date: " << file.create_date;
    std::cout << std::endl;
  }
}

void ClientApp::downloadFile() {
  showFiles();

  std::cout << "Choice file: ";
  int file;
  std::cin >> file;

  try {
    app.DownloadFile(file, &ClientApp::downloadFileCallbackOk, &ClientApp::downloadFileCallbackError);
  } catch (FileIdException &er) {
    std::cout << er.what() << std::endl;
  } catch (FileDownloadedException &er) {
    std::cout << er.what() << std::endl;
  }
}

void ClientApp::downloadFileCallbackOk() {
  std::cout << "downloadFileCallbackOk: " << "download complete" << std::endl;
}

void ClientApp::downloadFileCallbackError(const std::string &msg) {
  std::cout << "downloadFileCallbackError: " << msg << std::endl;
}

void ClientApp::showEvents() {
  std::cout << "Events: " << std::endl;

//  auto events = app.GetEvents();

//  for (auto &&ev: events) {
//    std::cout << "Event: " <<
//  }

  // TODO print events
}

void ClientApp::saveEvents() {
  app.SaveEvents(&ClientApp::saveEventsCallbackOk, &ClientApp::saveEventsCallbackError);
}

void ClientApp::saveEventsCallbackOk() {
  std::cout << "saveEventsCallbackOk: " << "upload complete" << std::endl;
}

void ClientApp::saveEventsCallbackError(const std::string &msg) {
  std::cout << "saveEventsCallbackError: " << msg << std::endl;
}

void ClientApp::uploadFile() {
  std::cout << "Chose file: ";

  std::string path = "/Users/s.alekhin/cloud/test.txt";
  std::cout << path << std::endl;
//  std::cin >> path;

  try {
    app.UploadFile(path, &ClientApp::uploadFileCallbackOk, &ClientApp::uploadFileCallbackError);
  } catch (FileNotExistsException &er) {
    std::cout << er.what() << std::endl;
  }
}

void ClientApp::uploadFileCallbackOk() {
  std::cout << "uploadFileCallbackOk: " << "upload complete" << std::endl;
}

void ClientApp::uploadFileCallbackError(const std::string &msg) {
  std::cout << "uploadFileCallbackError: " << msg << std::endl;
}