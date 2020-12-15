#include "Commands.h"
#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <utility>

namespace pt = boost::property_tree;

BaseCommand::BaseCommand(std::function<void()> callbackOk,
                         std::function<void(const std::string &msg)> callbackError,
                         std::shared_ptr<InternalDB> internalDB)
    : callbackOk(std::move(callbackOk)),
      callbackError(std::move(callbackError)),
      _internalDB(std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "BaseCommand: create command";
}

RefreshCommand::RefreshCommand(std::function<void()> callbackOk,
                               std::function<void(const std::string &msg)> callbackError,
                               std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: create command";
}

void RefreshCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: do";

  auto syncConfig = ClientConfig::getSyncConfig();

  // TODO refreshCommand Do Meta from Internal DB (userId, lastUpdate)
  auto userDate = UserDate{.userId = 1, .date = "31.12.1970"};

  auto requestSerializer = SerializerUserDate(0, userDate);

  auto sync = ClientNetwork();
  pt::ptree response;
  try {
    sync.Connect(syncConfig.host, syncConfig.port);
    sync.SendJSON(requestSerializer.GetJson());
    response = sync.ReceiveJSON();
  } catch (ClientNetworkExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "RefreshCommand: " << er.what();
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss;
  pt::write_json(ss, response);
  std::cout << ss.str() << std::endl;
  // test

  try {
    BOOST_LOG_TRIVIAL(info) << "RefreshCommand: parse response";
    auto responseSerializer = SerializerFileInfo(response);
    auto fileMeta = responseSerializer.GetFileMeta();

    // TODO refreshCommand Do Insert file to internal DB

    callbackOk();
    return;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(info) << "RefreshCommand: " << er.what();
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
    return;
  }
}

DownloadFileCommand::DownloadFileCommand(std::function<void()> callbackOk,
                                         std::function<void(const std::string &msg)> callbackError,
                                         std::shared_ptr<InternalDB> internalDB,
                                         Files &file)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)),
      _file(file) {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: create command";
}

void DownloadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: do";
  // TODO downloadCommand Do

  auto storageConfig = ClientConfig::getStorageConfig();

  // TODO downloadCommand Do UserChunk vector from InternalDB
  auto vec = std::vector<UserChunk>();
  int id = 1;
  for (int i = 1; i < 3; ++i) {
    auto userChunk = UserChunk{.userId = id, .chunkId = i};
    vec.push_back(userChunk);
  }

  auto requestSerializer = SerializerUserChunk(0, vec);

  auto storage = ClientNetwork();
  pt::ptree response;
  try {
    storage.Connect(storageConfig.host, storageConfig.port);
    storage.SendJSON(requestSerializer.GetJson());
    response = storage.ReceiveJSON();
  } catch (ClientNetworkExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "DownloadFileCommand: " << er.what();
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss;
  pt::write_json(ss, response);
  std::cout << ss.str() << std::endl;
  // test

  try {
    BOOST_LOG_TRIVIAL(info) << "DownloadFileCommand: parse response";
    auto responseSerializer = SerializerChunk(response);
    auto chunks = responseSerializer.GetChunk();

    // TODO downloadCommand Do Chunker create file

    callbackOk();
    return;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(error) << "DownloadFileCommand: " << er.what();
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
    return;
  }
}

CreateFileCommand::CreateFileCommand(std::function<void()> callbackOk,
                                     std::function<void(const std::string &msg)> callbackError,
                                     std::shared_ptr<InternalDB> internalDB,
                                     fs::path path)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)),
      _filePath(std::move(path)) {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: create command";
}

void CreateFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: do";
  // TODO createFileCommand Do

  auto storageConfig = ClientConfig::getStorageConfig();
  auto syncConfig = ClientConfig::getSyncConfig();

  std::cout << _filePath.string() << std::endl;

  // Indexer -> fs index file and insert to Internal DB (get file id) -> return file

  // File -> fs chunk file -> return Chunk


  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = true)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  callbackError("test");
}

RemoveFileCommand::RemoveFileCommand(std::function<void()> callbackOk,
                                     std::function<void(const std::string &msg)> callbackError,
                                     std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "RemoveFileCommand: create command";
}

void RemoveFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RemoveFileCommand: do";
  // TODO removeFileCommand Do

  auto syncConfig = ClientConfig::getSyncConfig();

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = false)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  callbackError("test");
}

RenameFileCommand::RenameFileCommand(std::function<void()> callbackOk,
                                     std::function<void(const std::string &msg)> callbackError,
                                     std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "RenameFileCommand: create command";
}

void RenameFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RenameFileCommand: do";
  // TODO removeFileCommand Do

  auto syncConfig = ClientConfig::getSyncConfig();

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = true)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  callbackError("test");
}

ModifyFileCommand::ModifyFileCommand(std::function<void()> callbackOk,
                                     std::function<void(const std::string &msg)> callbackError,
                                     std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "ModifyFileCommand: create command";
}

void ModifyFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "ModifyFileCommand: do";
  // TODO modifyFileCommand Do

  auto storageConfig = ClientConfig::getStorageConfig();
  auto syncConfig = ClientConfig::getSyncConfig();

  // chunk file
  // create chunkmeta
  // create filechunksmeta (isCurrent = true)

  // serialize allData
  // storageNetwork -> uploadChunks(Chunk ptree)

  // serialize MetaInfo
  // syncNetwork -> uploadMeta

  callbackError("test");
}