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

  auto userDate = _internalDB->GetLastUpdate();
  auto requestSerializer = SerializerUserDate(0, userDate);

  auto syncConfig = ClientConfig::getSyncConfig();
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
    auto fileInfo = responseSerializer.GetFileInfo();

    // TODO refreshCommand check is working
//    _internalDB->InsertFileInfo(fileInfo);

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
                                         FileMeta &file)
    : BaseCommand(std::move(callbackOk), std::move(callbackError), std::move(internalDB)),
      _file(file) {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: create command";
}

void DownloadFileCommand::Do() {
  BOOST_LOG_TRIVIAL(debug) << "DownloadFileCommand: do";

  auto userChunkVector = _internalDB->GetUsersChunks(_file.fileId);
  auto requestSerializer = SerializerUserChunk(0, userChunkVector);

  auto storageConfig = ClientConfig::getStorageConfig();
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
  BOOST_LOG_TRIVIAL(debug) << "CreateFileCommand: do";
  // TODO createFileCommand Do

//  auto storageConfig = ClientConfig::getStorageConfig();
//  auto syncConfig = ClientConfig::getSyncConfig();

  std::cout << _filePath.string() << std::endl;

  std::vector<FileInfo> fileInfo;
  auto file = FileMeta{.fileId = 1,
      .version = 1,
      .fileName = "test",
      .fileExtension = "txt",
      .filePath = "static/",
      .fileSize = 1,
      .chunksCount = 1,
      .isCurrent = true,
      .isDeleted = false,
      .updateDate = "31.12.1970",
      .createDate = "31.12.1970"};

  std::vector<ChunkMeta> chunksMetaVector;
  for (int i = 1; i < 3; ++i) {
    auto chunkMeta = ChunkMeta{.chunkId = i};
    chunksMetaVector.push_back(chunkMeta);
  }

  std::vector<FileChunksMeta> fileChunksMetaVector;
  for (int i = 1; i < 3; ++i) {
    auto fileChunkMeta = FileChunksMeta{.chunkId = i, .chunkOrder = i};
    fileChunksMetaVector.push_back(fileChunkMeta);
  }
  fileInfo.push_back(FileInfo{.userId = 1, .file = file, .chunkMeta = chunksMetaVector, .fileChunksMeta = fileChunksMetaVector});

  auto requestSerializer = SerializerFileInfo(0, fileInfo);

  auto syncConfig = ClientConfig::getSyncConfig();
  auto sync = ClientNetwork();
  pt::ptree response;
  try {
    sync.Connect(syncConfig.host, syncConfig.port);
    sync.SendJSON(requestSerializer.GetJson());
    response = sync.ReceiveJSON();

  } catch (ClientNetworkExceptions &er) {
    BOOST_LOG_TRIVIAL(error) << "FileCommand: " << er.what();
    callbackError(er.what());
    return;
  }

  // test
  std::stringstream ss;
  pt::write_json(ss, response);
  std::cout << ss.str() << std::endl;
  // test

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
