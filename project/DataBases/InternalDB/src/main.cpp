//
// Created by Ivan Gorshkov on 23.11.2020.
//
#include <iostream>
#include "InternalDB.h"

int main(int argc, const char** argv) {
  std::cout << argc << argv << std::endl;
  InternalDB myDB("myDB.sqlite");
  myDB.ExistUser();
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
  myDB.InsertFile(f);
  myDB.SelectFile(2);
  Chunks c;
  c.idFile = 1;
  c.chunkSize = 2;
  c.id = 1;
  c.rapidHash = "asd";
  c.staticHash = "adsd";
  myDB.InsertChunk(c);
  std::cout << myDB.SelectUserPassword() << std::endl;

  return 0;
}