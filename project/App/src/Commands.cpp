#include "Commands.h"
#include <iostream>

RefreshCommand::RefreshCommand() {
  std::cout << "RefreshCommand" << std::endl;
}

void RefreshCommand::Do() {
  // TODO refreshCommand Do

  // GetDate from internal DB

  // serialize date
  // metaNetwork -> get new meta(date ptree)

  // deserialize ptree to struct
  // send struct to Internal DB
}

DownloadFileCommand::DownloadFileCommand() {
  std::cout << "DownloadFileCommand" << std::endl;
}

void DownloadFileCommand::Do() {
  // TODO downloadCommand Do

  // GetFileChunks from Internal DB

  // serialize fileChunks
  // storageNetwork -> downloadChunk(userChunk ptree)

  // deserialize ptree to struct
  // send Chunker to create file
}

CreateFileCommand::CreateFileCommand() {
  std::cout << "CreateFileCommand" << std::endl;
}

void CreateFileCommand::Do() {
  // TODO createFileCommand Do

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = true)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  // callback ok
}

RemoveFileCommand::RemoveFileCommand() {
  std::cout << "RemoveFileCommand" << std::endl;
}

void RemoveFileCommand::Do() {
  // TODO removeFileCommand Do

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = false)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  // callback ok
}

RenameFileCommand::RenameFileCommand() {
  std::cout << "RenameFileCommand" << std::endl;
}

void RenameFileCommand::Do() {
  // TODO removeFileCommand Do

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = true)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  // callback ok
}

ModifyFileCommand::ModifyFileCommand() {
  std::cout << "ModifyFileCommand" << std::endl;
}

void ModifyFileCommand::Do() {
  // TODO modifyFileCommand Do

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = true)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  // callback ok
}