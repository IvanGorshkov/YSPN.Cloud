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
  explicit SerializerFileInfo(int requestId, std::vector<FileInfo> fileMetaVector);
  explicit SerializerFileInfo(const pt::ptree &val);

  int GetRequestId() const;
  std::vector<FileInfo> GetFileInfo() noexcept(false);
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;

 private:
  int _requestId{};
  std::vector<FileInfo> _fileInfoVector;
  pt::ptree _json{};
};
