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

void BaseCommand::connect(ClientNetwork &network, NetworkConfig &config) {
  try {
    network.Connect(config.host, config.port);
  } catch (ConnectServerException &er) {
    BOOST_LOG_TRIVIAL(error) << "connect: " << er.what();
    throw NetworkException(er.what());
  }
}

void BaseCommand::send(ClientNetwork &network, const pt::ptree &json) {
  try {
    network.SendJSON(json);
  } catch (SendToServerParseJsonException &er) {
    BOOST_LOG_TRIVIAL(error) << "send: " << er.what();
    throw NetworkException(er.what());

  } catch (SendToServerException &er) {
    BOOST_LOG_TRIVIAL(error) << "send: " << er.what();
    throw NetworkException(er.what());
  }
}

void BaseCommand::receive(ClientNetwork &network, pt::ptree &json) {
  try {
    json = network.ReceiveJSON();
  } catch (ReadFromServerException &er) {
    BOOST_LOG_TRIVIAL(error) << "receive: " << er.what();
    throw NetworkException(er.what());

  } catch (ReadFromServerParseJsonException &er) {
    BOOST_LOG_TRIVIAL(error) << "receive: " << er.what();
    throw NetworkException(er.what());
  }
}

RefreshCommand::RefreshCommand(std::function<void()> callbackOk,
                               std::function<void(const std::string &msg)> callbackError,
                               std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: create command";
}

void RefreshCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: do";
  // TODO refreshCommand Do

  auto syncConfig = ClientConfig::getSyncConfig();

  // Meta from Internal DB (userId, lastUpdate)
  auto userDate = UserDate{.userId = 1, .date = "31.12.1970"};
  // Meta from Internal DB (userId, lastUpdate)
  auto requestSerializer = SerializerUserDate(0, userDate);

  auto sync = ClientNetwork();
  pt::ptree response;
  try {
    connect(sync, syncConfig);
    send(sync, requestSerializer.GetJson());
    receive(sync, response);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss;
  pt::write_json(ss, response);
  std::cout << ss.str() << std::endl;
  // test

  try {
    auto responseSerializer = SerializerFileMeta(response);
    auto fileMeta = responseSerializer.GetFileMeta();

    // Insert file to internal DB

    callbackOk();
    return;

  } catch (ParseException &er) {
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
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

  // Get UserChunk vector from InternalDB
  auto vec = std::vector<UserChunk>();
  int id = 1;
  for (int i = 1; i < 3; ++i) {
    auto userChunk = UserChunk{.userId = id, .chunkId = i};
    vec.push_back(userChunk);
  }
  // Get UserChunk vector from InternalDB
  auto requestSerializer = SerializerUserChunk(0, vec);

  auto storage = ClientNetwork();
  pt::ptree response;
  try {
    connect(storage, storageConfig);
    send(storage, requestSerializer.GetJson());
    receive(storage, response);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss;
  pt::write_json(ss, response);
  std::cout << ss.str() << std::endl;
  // test

  try {
    auto responseSerializer = SerializerChunk(response);
    auto chunks = responseSerializer.GetChunk();

    // Chunker create file

    callbackOk();
    return;

  } catch (ParseException &er) {
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
  }
}

CreateFileCommand::CreateFileCommand(std::function<void()> callbackOk,
                                     std::function<void(const std::string &msg)> callbackError,
                                     std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: create command";
}

void CreateFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: do";
  // TODO createFileCommand Do

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