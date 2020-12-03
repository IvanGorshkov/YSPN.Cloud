#pragma once

#include "SerializerInterface.h"
#include "SerializerExceptions.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

namespace pt = boost::property_tree;

struct UserChunk {
  UserChunk() = default;

  UserChunk(int userId, int chunkId)
      : userId(userId),
        chunkId(chunkId) {
  }

  int userId{};
  int chunkId{};
};

class SerializerUserChunk : public SerializerInterface {
 public:
  SerializerUserChunk() = default;
  explicit SerializerUserChunk(std::vector<UserChunk> val);
  explicit SerializerUserChunk(const pt::ptree &val);

  std::vector<UserChunk> GetChunk() noexcept(false);
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;

 private:
  std::vector<UserChunk> _chunkVector;
  pt::ptree _json;
};
