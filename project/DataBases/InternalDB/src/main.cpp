//
// Created by Ivan Gorshkov on 23.11.2020.
//
#include <iostream>
#include "InternalDB.h"
#include <vector>

int main() {
  InternalDB myDB("myDB.sqlite");

  //myDB.DeleteUser(233);
  myDB.UpdateSyncFolder("asd");
  Files f;
  f.file_name = "hello";
  f.file_extention = "hello";
  f.create_date = "hello";
  f.update_date = "hello";
  f.is_download = false;
  f.version = 1;
  f.count_chunks = 0;
  f.file_path = "fuck";
  f.file_size = 232;
  std::vector<Files> files;
  files.push_back(f);
  myDB.InsertFile(files);
  myDB.SelectFile(2);
  Chunks c;
  c.idFile = 1;
  c.chunkSize = 2;
  c.id = 1;
  c.rapidHash = "asd";
  c.staticHash = "adsd";
  myDB.InsertChunk(c);
  User u;
  u.userId = 1;
  u.lastUpdate = "popusk";
  u.syncFolder = "popusk";
  u.deviceName = "popusk";
  u.deviceId = 3;
  u.password = "3";
  u.login = "3";
  //myDB.InsertUser(u);

  std::cout << myDB.GetLastUpdate() << std::endl;
  myDB.SaveLastUpdate();
  std::cout << myDB.GetLastUpdate() << std::endl;
  return 0;
}