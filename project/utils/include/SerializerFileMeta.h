#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include "structs/FileMeta.h"
#include "SerializerInterface.h"
#include "SerializerExceptions.h"

namespace pt = boost::property_tree;

class SerializerFileMeta : public SerializerInterface {
 public:
  SerializerFileMeta() = default;
  explicit SerializerFileMeta(int requestId, int userId, FileMeta fileMeta);
  explicit SerializerFileMeta(const pt::ptree &val);

  int GetRequestId() const;
  FileMeta GetFileMeta() noexcept(false);
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;

 private:
  int _requestId{};
  int _userId{};
  FileMeta _fileMeta;
  pt::ptree _json{};
};
