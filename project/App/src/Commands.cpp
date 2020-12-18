#include "Commands.h"
#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <utility>
#include <variant>

namespace pt = boost::property_tree;

BaseCommand::BaseCommand(std::function<void()> callbackOk,
                         std::function<void(const std::string &msg)> callbackError,
                         std::shared_ptr<InternalDB> internalDB)
    : callbackOk(std::move(callbackOk)),
      callbackError(std::move(callbackError)),
      _internalDB(std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "BaseCommand: create command";
}

void BaseCommand::connect(ClientNetwork &network,
                          const NetworkConfig &config,
                          const pt::ptree &request,
                          pt::ptree &response) noexcept(false) {
  try {
    network.Connect(config.host, config.port);
    network.SendJSON(request);
    response = network.ReceiveJSON();
    BOOST_LOG_TRIVIAL(info) << "BaseCommand: send and receive";

  } catch (ClientNetworkExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "BaseCommand: " << er.what();
    throw NetworkException(er.what());
  }
  network.Disconnect();
}

RefreshCommand::RefreshCommand(std::function<void()> callbackOk,
                               std::function<void(const std::string &msg)> callbackError,
                               std::shared_ptr<InternalDB> internalDB)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: create command";
}

void RefreshCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "RefreshCommand: Do";

  auto syncConfig = ClientConfig::getSyncConfig();
  auto network = ClientNetwork();

  auto userDate = _internalDB->GetLastUpdate();
  auto request = SerializerUserDate(0, userDate).GetJson();

  pt::ptree response;
  try {
    connect(network, syncConfig, request, response);
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
    auto fileInfo = SerializerFileInfo(response).GetFileInfo();
    BOOST_LOG_TRIVIAL(info) << "RefreshCommand: get fileInfo";

    // TODO refreshCommand check is working
//    _internalDB->InsertFileInfo(fileInfo);

    callbackOk();
    return;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(error) << "RefreshCommand: " << er.what();
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
    return;
  }
}

DownloadFileCommand::DownloadFileCommand(std::function<void()> callbackOk,
                                         std::function<void(const std::string &msg)> callbackError,
                                         std::shared_ptr<InternalDB> internalDB,
                                         FileMeta &file)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)),
      _file(file) {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: create command";
}

void DownloadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: Do";

  auto storageConfig = ClientConfig::getStorageConfig();
  auto network = ClientNetwork();

  auto userChunkVector = _internalDB->GetUsersChunks(_file.fileId);
  auto request = SerializerUserChunk(0, userChunkVector).GetJson();

  pt::ptree response;
  try {
    connect(network, storageConfig, request, response);
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
    auto chunks = SerializerChunk(response).GetChunk();
    BOOST_LOG_TRIVIAL(info) << "DownloadFileCommand: get chunks";

    std::string filePath = _internalDB->GetSyncFolder() + _file.filePath + _file.fileName + '.' + _file.fileExtension;
    std::cout << filePath << std::endl;

    File file(filePath);
    Chunker chunker(file);
    chunker.MergeFile(chunks);

    // TODO internalDB isDownloaded = true

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

FileCommand::FileCommand(std::function<void()> callbackOk,
                         std::function<void(const std::string &msg)> callbackError,
                         std::shared_ptr<InternalDB> internalDB,
                         fs::path path,
                         bool isDeleted)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)),
      _filePath(std::move(path)),
      _isDeleted(isDeleted) {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: create command";
}

void FileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "FileCommand: do";

  auto storageConfig = ClientConfig::getStorageConfig();
  auto syncConfig = ClientConfig::getSyncConfig();
  auto network = ClientNetwork();

  File file(_filePath.string());
  Chunker chunker(file);
  auto chunkVector = chunker.ChunkFile();

  Indexer indexer(_internalDB);
  auto fileMeta = indexer.GetFileMeta(_filePath, _isDeleted);
  auto fileInfo = indexer.GetFileInfo(fileMeta, chunkVector);
  auto storageRequest = SerializerChunk(0, chunkVector).GetJson();

  pt::ptree responseStorage;
  try {
    connect(network, storageConfig, storageRequest, responseStorage);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss;
  pt::write_json(ss, responseStorage);
  std::cout << "Storage: " << ss.str() << std::endl;
  // test

  auto responseStorageStatus = SerializerAnswer(responseStorage).GetStatus();
  bool isError = false;
  std::visit(overloaded{
      [&](const StatusOk &val) {
        BOOST_LOG_TRIVIAL(info) << "FileCommand: Status Ok sync";
      },
      [&](const StatusError &val) {
        BOOST_LOG_TRIVIAL(error) << "FileCommand: Status Error sync";
        auto error = std::get<StatusError>(responseStorageStatus);
        callbackError(error.msg);
        isError = true;
      }
  }, responseStorageStatus);
  if (isError) {
    return;
  }

  auto syncRequest = SerializerFileInfo(0, fileInfo).GetJson();
  pt::ptree responseSync;
  try {
    connect(network, syncConfig, syncRequest, responseSync);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss2;
  pt::write_json(ss2, responseSync);
  std::cout << "Sync: " << ss2.str() << std::endl;
  // test

  auto responseSyncStatus = SerializerAnswer(responseSync).GetStatus();
  std::visit(overloaded{
      [&](const StatusOk &val) {
        BOOST_LOG_TRIVIAL(info) << "FileCommand: Status Ok sync";
      },
      [&](const StatusError &val) {
        BOOST_LOG_TRIVIAL(error) << "FileCommand: Status Error sync";
        auto error = std::get<StatusError>(responseSyncStatus);
        callbackError(error.msg);
      }
  }, responseSyncStatus);
}
