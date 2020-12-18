#pragma once

#include <boost/property_tree/ptree.hpp>
#include "Command.h"
#include "MetaDataDB.h"
#include "PostgresExceptions.h"
#include "SerializerAnswer.h"
#include "SerializerUserDate.h"
#include "SerializerFileInfo.h"

namespace pt = boost::property_tree;

class DataUpdateCommand : public Command {
 public:
  explicit DataUpdateCommand(std::shared_ptr<pt::ptree> &request);
  std::shared_ptr<pt::ptree> Do() override;

 private:
  std::shared_ptr<pt::ptree> _request;
  MetaDataDB &_db;
  SerializerUserDate _userDate;
};

class UploadFileCommand : public Command {
 public:
  explicit UploadFileCommand(std::shared_ptr<pt::ptree> &request);
  std::shared_ptr<pt::ptree> Do() override;

 private:
  std::shared_ptr<pt::ptree> _request;
  MetaDataDB &_db;
  SerializerFileInfo _fileInfo;
};