#include "SerializerChunk.h"
#include <iostream>

SerializerChunk::SerializerChunk(std::vector<Chunk> val) noexcept
    : chunkVector(std::move(val)) {
}

SerializerChunk::SerializerChunk(const pt::ptree& val) noexcept
    : json(val) {
}

std::vector<Chunk> SerializerChunk::GetChunk() {
  if (chunkVector.empty()) {
    deserialize();
    printVector();
  }

  return std::move(chunkVector);
}

pt::ptree SerializerChunk::GetJson() {
  if (json.empty()) {
    serialize();
    printJson();
  }

  return json;
}

void SerializerChunk::serialize() {
  json.put("command", COMMAND);
  pt::ptree data;
  for (auto &el : chunkVector) {
    pt::ptree child;
    child.put("userId", el.userId);
    child.put("chunkId", el.chunkId);
    child.put("sHash", el.sHash);
    child.put("rHash", el.rHash);
    child.put("data", el.data);

    data.push_back(std::make_pair("", child));
  }

  json.add_child("data", data);
}

void SerializerChunk::deserialize() {
  for (auto &val : json.get_child("data")) {
    try {
      chunkVector.emplace_back(val.second.get<int>("userId"),
                               val.second.get<int>("chunkId"),
                               val.second.get<std::string>("sHash"),
                               val.second.get<std::string>("rHash"),
                               val.second.get<std::string>("data"));
    } catch (pt::ptree_error &er) {
      std::cout << er.what() << std::endl;
    }
  }
}

void SerializerChunk::printVector() {
  for (auto &el : chunkVector) {
    std::cout << "UserId = " << el.userId << std::endl;
    std::cout << "chinkId = " << el.chunkId << std::endl;
    std::cout << "sHash = " << el.sHash << std::endl;
    std::cout << "rHash = " << el.rHash << std::endl;
    std::cout << "data = " << el.data << std::endl;
    std::cout << std::endl;
  }
}

void SerializerChunk::printJson() {
  for (auto &val : json.get_child("data")) {
    std::cout << "UserId = " << val.second.get<int>("userId") << std::endl;
    std::cout << "chinkId = " << val.second.get<int>("chunkId") << std::endl;
    std::cout << "sHash = " << val.second.get<std::string>("sHash") << std::endl;
    std::cout << "rHash = " << val.second.get<std::string>("rHash") << std::endl;
    std::cout << "data = " << val.second.get<std::string>("data") << std::endl;
    std::cout << std::endl;
  }
}
