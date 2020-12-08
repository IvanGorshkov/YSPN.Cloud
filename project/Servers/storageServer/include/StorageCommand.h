#pragma once

#include <boost/property_tree/ptree.hpp>
#include "Command.h"
#include "MongoDB.h"
#include "SerializerAnswer.h"
#include "SerializerChunk.h"
#include "SerializerUserChunk.h"

namespace pt = boost::property_tree;

class UploadChunkCommand : public Command {
 public:
  explicit UploadChunkCommand(std::shared_ptr<pt::ptree> &request);
  std::shared_ptr<pt::ptree> Do() override;

 private:
  std::shared_ptr<pt::ptree> _request;
  MongoDB &_db;
  SerializerChunk _chunk;
};

class DownloadChunkCommand : public Command {
 public:
  explicit DownloadChunkCommand(std::shared_ptr<pt::ptree> &request);
  std::shared_ptr<pt::ptree> Do() override;

 private:
  std::shared_ptr<pt::ptree> _request;
  MongoDB &_db;
  SerializerUserChunk _userChunk;
};