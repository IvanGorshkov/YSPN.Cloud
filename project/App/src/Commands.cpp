#include "Commands.h"
#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <utility>
#include <variant>

namespace pt = boost::property_tree;

BaseCommand::BaseCommand(std::function<void(const std::string &msg)> callbackOk,
                         std::function<void(const std::string &msg)> callbackError,
                         std::shared_ptr<InternalDB> internalDB)
    : callbackOk(std::move(callbackOk)),
      callbackError(std::move(callbackError)),
      _internalDB(std::move(internalDB)) {
  BOOST_LOG_TRIVIAL(debug) << "BaseCommand: create command";
}

void BaseCommand::sendAndReceive(ClientNetwork &network,
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

RefreshCommand::RefreshCommand(std::function<void(const std::string &msg)> callbackOk,
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
    sendAndReceive(network, syncConfig, request, response);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

  try {
    auto fileInfo = SerializerFileInfo(response).GetFileInfo();
    BOOST_LOG_TRIVIAL(info) << "RefreshCommand: get fileInfo";

    std::for_each(fileInfo.begin(), fileInfo.end(), [&](FileInfo &oneFileInfo) {
      oneFileInfo.file.isDownload = false;

      if (oneFileInfo.file.isDeleted) {
        File::Delete(_internalDB->GetSyncFolder() + oneFileInfo.file.GetFilePath());
        _internalDB->DeleteFile(oneFileInfo.file.fileId);
        return;
      }

      if (_internalDB->IsFileExist(oneFileInfo.file.fileId)) {
        auto fileDB = _internalDB->SelectFile(oneFileInfo.file.fileId);
        if (fileDB.isDownload) {
          File::Delete(_internalDB->GetSyncFolder() + fileDB.GetFilePath());
        }
      }
      _internalDB->InsertOrUpdateFileInfo(oneFileInfo);
    });

    _internalDB->SaveLastUpdate();
    callbackOk("Обновление завершено");
    return;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(error) << "RefreshCommand: " << er.what();
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
    return;
  }
}

DownloadFileCommand::DownloadFileCommand(std::function<void(const std::string &msg)> callbackOk,
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

  auto userChunkVector = _internalDB->GetFileChunks(_file.fileId);
  auto request = SerializerUserChunk(0, userChunkVector).GetJson();

  pt::ptree response;
  try {
    sendAndReceive(network, storageConfig, request, response);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

  try {
    auto chunks = SerializerChunk(response).GetChunk();
    BOOST_LOG_TRIVIAL(info) << "DownloadFileCommand: get chunks";

    auto filePath = _internalDB->GetSyncFolder() + _file.GetFilePath();
    BOOST_LOG_TRIVIAL(info) << "DownloadFileCommand: download file " << filePath;

    File file(filePath);
    Chunker chunker(file);
    chunker.MergeFile(chunks);

    _internalDB->DownloadFile(_file.fileId);
    callbackOk("Загрузка завершена");
    return;

  } catch (ParseException &er) {
    BOOST_LOG_TRIVIAL(error) << "DownloadFileCommand: " << er.what();
    auto responseSerializer = SerializerAnswer(response);
    auto error = std::get<StatusError>(responseSerializer.GetStatus());
    callbackError(error.msg);
    return;
  }
}

FileCommand::FileCommand(std::function<void(const std::string &msg)> callbackOk,
                         std::function<void(const std::string &msg)> callbackError,
                         std::shared_ptr<InternalDB> internalDB,
                         fs::path path,
                         boost::optional<fs::path> newPath,
                         bool isDeleted)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)),
      _filePath(std::move(path)),
      _newFilePath(std::move(newPath)),
      _isDeleted(isDeleted) {
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: create command";
}

void FileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "FileCommand: do";

  auto storageConfig = ClientConfig::getStorageConfig();
  auto syncConfig = ClientConfig::getSyncConfig();
  auto network = ClientNetwork();

  Indexer indexer(_internalDB);
  auto fileMeta = indexer.GetFileMeta(_filePath, _isDeleted, _newFilePath);
  if (fileMeta.isDeleted) {
    _internalDB->DeleteFile(fileMeta.fileId);
  }
  std::vector<Chunk> chunkVector;

  if (!fileMeta.isDeleted) {
    File file(_filePath.string());
    Chunker chunker(file);
    chunkVector = chunker.ChunkFile();
  }
  auto fileInfo = indexer.GetFileInfo(fileMeta, chunkVector);

  if (!fileMeta.isDeleted) {
    auto storageRequest = SerializerChunk(0, chunkVector).GetJson();
    pt::ptree responseStorage;
    try {
      sendAndReceive(network, storageConfig, storageRequest, responseStorage);
    } catch (NetworkException &er) {
      callbackError(er.what());
      return;
    }

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
  }

  auto syncRequest = SerializerFileInfo(0, fileInfo).GetJson();
  pt::ptree responseSync;
  try {
    sendAndReceive(network, syncConfig, syncRequest, responseSync);
  } catch (NetworkException &er) {
    callbackError(er.what());
    return;
  }

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

  callbackOk("Файл обновлен");
}