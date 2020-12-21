#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include "InternalDB.h"
#include "ClientNetwork.h"
#include "ClientNetworkExceptions.h"
#include "CommandsExceptions.h"
#include "ClientConfig.h"
#include "SerializerAnswer.h"
#include "SerializerChunk.h"
#include "SerializerUserChunk.h"
#include "SerializerUserDate.h"
#include "SerializerFileInfo.h"
#include "SerializerExceptions.h"
#include "Indexer.h"
#include "File.h"
#include "Chunker.h"

namespace fs = boost::filesystem;

class BaseCommand {
 public:
  explicit BaseCommand(std::function<void()> callbackOk,
                       std::function<void(const std::string &msg)> callbackError,
                       std::shared_ptr<InternalDB> internalDB);
  virtual ~BaseCommand() = default;
  virtual void Do() = 0;

 protected:
  static void connect(ClientNetwork &network,
                      const NetworkConfig &config,
                      const pt::ptree &request,
                      pt::ptree &response) noexcept(false);

 protected:
  std::function<void()> callbackOk;
  std::function<void(const std::string &msg)> callbackError;

 protected:
  std::shared_ptr<InternalDB> _internalDB;
};

class RefreshCommand : public BaseCommand {
 public:
  explicit RefreshCommand(std::function<void()> callbackOk,
                          std::function<void(const std::string &msg)> callbackError,
                          std::shared_ptr<InternalDB> internalDB);
  void Do() override;
};

class DownloadFileCommand : public BaseCommand {
 public:
  explicit DownloadFileCommand(std::function<void()> callbackOk,
                               std::function<void(const std::string &msg)> callbackError,
                               std::shared_ptr<InternalDB> internalDB,
                               FileMeta &file);
  void Do() override;

 private:
  FileMeta _file;
};

class FileCommand : public BaseCommand {
 public:
  explicit FileCommand(std::function<void()> callbackOk,
                       std::function<void(const std::string &msg)> callbackError,
                       std::shared_ptr<InternalDB> internalDB,
                       fs::path path,
                       boost::optional<fs::path> newPath = boost::none,
                       bool isDeleted = false);

  void Do() override;

 private:
  fs::path _filePath;
  boost::optional<fs::path> _newFilePath;
  bool _isDeleted;
};