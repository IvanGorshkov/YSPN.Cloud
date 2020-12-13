#include "Commands.h"
#include <boost/log/trivial.hpp>
#include <utility>

namespace pt = boost::property_tree;

RefreshCommand::RefreshCommand(std::shared_ptr<InternalDB> db)
    : _internalDB(std::move(db)) {
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

DownloadFileCommand::DownloadFileCommand(std::function<void(const std::string &msg)> callbackOk,
                                         std::function<void(const std::string &msg)> callbackError,
                                         std::shared_ptr<ClientNetwork> storageNetwork,
                                         Files &file)
    : callbackOk(std::move(callbackOk)),
      callbackError(std::move(callbackError)),
      _storageNetwork(std::move(storageNetwork)),
      _file(file) {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: create command";
}

void DownloadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: do";
  // TODO downloadCommand Do

  // Get UserChunk vector from InternalDB

  auto vec = std::vector<UserChunk>();

  int id = 2;
  for (int i = 0; i < 2; ++i) {
    vec.emplace_back(id, i);
  }

  // Get UserChunk vector from InternalDB

  auto requestSerializer = SerializerUserChunk(0, vec);
  _storageNetwork->SendJSON(requestSerializer.GetJson());

  auto response = _storageNetwork->ReceiveJSON();

  try {
    auto responceSerializer = SerializerChunk(response);
    auto chunks = responceSerializer.GetChunk();
    // Chunker create file
    callbackOk("download complete");

  } catch (ParseException &er) {
    auto responceSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responceSerializer.GetStatus());
    callbackError(error.msg);
  }
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