#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include "SerializerInterface.h"
#include "SerializerExceptions.h"

namespace pt = boost::property_tree;

struct Chunk {
  Chunk() = default;
  Chunk(int userId, int chunkId, std::string sHash, std::string rHash, std::string data)
      : userId(userId),
        chunkId(chunkId),
        sHash(std::move(sHash)),
        rHash(std::move(rHash)),
        data(std::move(data)) {
  }

  int userId{};
  int chunkId{};
  std::string sHash;
  std::string rHash;
  std::string data;
};

class SerializerChunk : public SerializerInterface {
 public:
  SerializerChunk() = default;
  explicit SerializerChunk(int requestId, std::vector<Chunk> val);
  explicit SerializerChunk(const pt::ptree &val);

  int GetRequestId() const;
  std::vector<Chunk> GetChunk() noexcept(false);
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;

 private:
  int _requestId{};
  std::vector<Chunk> _chunkVector;
  pt::ptree _json{};
};
