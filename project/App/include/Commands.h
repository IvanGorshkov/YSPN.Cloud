#pragma once

#include "InternalDB.h"
#include <string>

class CommandInterface {
 public:
  virtual ~CommandInterface() = default;
  virtual void Do() = 0;
};

class RefreshCommand : public CommandInterface {
 public:
  explicit RefreshCommand(const std::shared_ptr<InternalDB> &db);
  void Do() override;

 private:
  std::shared_ptr<InternalDB> _internalDB;
};

class DownloadFileCommand : public CommandInterface {
 public:
  explicit DownloadFileCommand();
  void Do() override;
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