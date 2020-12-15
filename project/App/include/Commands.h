#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include "InternalDB.h"
#include "ClientNetwork.h"
#include "ClientNetworkExceptions.h"
#include "CommandsExceptions.h"
#include "ClientConfig.h"
#include "Files.h"
#include "SerializerAnswer.h"
#include "SerializerChunk.h"
#include "SerializerUserChunk.h"
#include "SerializerUserDate.h"
#include "SerializerFileInfo.h"
#include "SerializerExceptions.h"

namespace fs = boost::filesystem;

class BaseCommand {
 public:
  explicit BaseCommand(std::function<void()> callbackOk,
                       std::function<void(const std::string &msg)> callbackError,
                       std::shared_ptr<InternalDB> internalDB);
  virtual ~BaseCommand() = default;
  virtual void Do() = 0;

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
                               Files &file);
  void Do() override;

 private:
  Files _file;
};

class CreateFileCommand : public BaseCommand {
 public:
  explicit CreateFileCommand(std::function<void()> callbackOk,
                             std::function<void(const std::string &msg)> callbackError,
                             std::shared_ptr<InternalDB> internalDB,
                             fs::path path);
  void Do() override;

 private:
  fs::path _filePath;
};

class RemoveFileCommand : public BaseCommand {
 public:
  explicit RemoveFileCommand(std::function<void()> callbackOk,
                             std::function<void(const std::string &msg)> callbackError,
                             std::shared_ptr<InternalDB> internalDB);
  void Do() override;
};

class RenameFileCommand : public BaseCommand {
 public:
  explicit RenameFileCommand(std::function<void()> callbackOk,
                             std::function<void(const std::string &msg)> callbackError,
                             std::shared_ptr<InternalDB> internalDB);
  void Do() override;
};

class ModifyFileCommand : public BaseCommand {
 public:
  explicit ModifyFileCommand(std::function<void()> callbackOk,
                             std::function<void(const std::string &msg)> callbackError,
                             std::shared_ptr<InternalDB> internalDB);
  void Do() override;
};