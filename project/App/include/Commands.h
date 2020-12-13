#pragma once

#include <string>
#include "InternalDB.h"
#include "ClientNetwork.h"
#include "Files.h"
#include "SerializerAnswer.h"
#include "SerializerChunk.h"
#include "SerializerUserChunk.h"
#include "SerializerExceptions.h"

class CommandInterface {
 public:
  virtual ~CommandInterface() = default;
  virtual void Do() = 0;
};

class RefreshCommand : public CommandInterface {
 public:
  explicit RefreshCommand(std::shared_ptr<InternalDB> db);
  void Do() override;

 private:
  std::shared_ptr<InternalDB> _internalDB;
};

class DownloadFileCommand : public CommandInterface {
 public:
  explicit DownloadFileCommand(std::function<void(const std::string &msg)> callbackOk,
                               std::function<void(const std::string &msg)> callbackError,
                               std::shared_ptr<ClientNetwork> _storageNetwork,
                               Files &file);
  void Do() override;

 private:
  std::function<void(const std::string &msg)> callbackOk;
  std::function<void(const std::string &msg)> callbackError;
  std::shared_ptr<ClientNetwork> _storageNetwork;
  Files _file;
};

class CreateFileCommand : public CommandInterface {
 public:
  explicit CreateFileCommand();
  void Do() override;
};

class RemoveFileCommand : public CommandInterface {
 public:
  explicit RemoveFileCommand();
  void Do() override;
};

class RenameFileCommand : public CommandInterface {
 public:
  explicit RenameFileCommand();
  void Do() override;
};

class ModifyFileCommand : public CommandInterface {
 public:
  explicit ModifyFileCommand();
  void Do() override;
};