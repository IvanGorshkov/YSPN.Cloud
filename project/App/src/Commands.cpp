#include "Commands.h"
#include <boost/log/trivial.hpp>

RefreshCommand::RefreshCommand(const std::shared_ptr<InternalDB> &db)
    : _internalDB(db) {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: create command";
}

void RefreshCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: do";

  // TODO refreshCommand Do

  auto date = _internalDB->GetLastUpdate();
  auto id = _internalDB->GetUserId();

  // GetDate from internal DB

  // serialize date
  // metaNetwork -> get new meta(date ptree)

  // deserialize ptree to struct
  // send struct to Internal DB
}

DownloadFileCommand::DownloadFileCommand() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: create command";
}

void DownloadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: do";
  // TODO downloadCommand Do

  // GetFileChunks from Internal DB

  // serialize fileChunks
  // storageNetwork -> downloadChunk(userChunk ptree)

  // deserialize ptree to struct
  // send Chunker to create file
}

CreateFileCommand::CreateFileCommand() {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: create command";
}

void CreateFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: do";
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
  BOOST_LOG_TRIVIAL(debug) << "RemoveFileCommand: create command";
}

void RemoveFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RemoveFileCommand: do";
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
  BOOST_LOG_TRIVIAL(debug) << "RenameFileCommand: create command";
}

void RenameFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RenameFileCommand: do";
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
  BOOST_LOG_TRIVIAL(debug) << "ModifyFileCommand: create command";
}

void ModifyFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "ModifyFileCommand: do";
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