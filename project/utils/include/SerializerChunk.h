#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <utility>
#include <memory>
#include "SerializerInterface.h"

#define COMMAND "AddChunk"

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
  explicit SerializerChunk(std::vector<Chunk> val) noexcept;
  explicit SerializerChunk(const pt::ptree& val) noexcept;

  std::vector<Chunk> GetChunk();
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;
  void printVector();
  void printJson();

 private:
  std::vector<Chunk> chunkVector;
  pt::ptree json;
};
