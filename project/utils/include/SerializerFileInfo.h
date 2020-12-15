#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include "structs/FileInfo.h"
#include "SerializerInterface.h"
#include "SerializerExceptions.h"

namespace pt = boost::property_tree;

class SerializerFileInfo : public SerializerInterface {
 public:
  SerializerFileInfo() = default;
  explicit SerializerFileInfo(int requestId, int userId, std::vector<FileInfo> fileMetaVector);
  explicit SerializerFileInfo(const pt::ptree &val);

  int GetRequestId() const;
  std::vector<FileInfo> GetFileMeta() noexcept(false);
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;

 private:
  int _requestId{};
  int _userId{};
  std::vector<FileInfo> _fileMetaVector;
  pt::ptree _json{};
};
