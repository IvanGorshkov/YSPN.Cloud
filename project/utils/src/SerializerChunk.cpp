#include "SerializerChunk.h"
#include <boost/log/trivial.hpp>

SerializerChunk::SerializerChunk(std::vector<Chunk> val)
    : _chunkVector(std::move(val)) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: create serializer chunk from vector";
}

SerializerChunk::SerializerChunk(const pt::ptree &val)
    : _json(val) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: create serializer chunk from json";
}

std::vector<Chunk> SerializerChunk::GetChunk() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: GetChunk";
  if (_chunkVector.empty()) {
    deserialize();
  }

  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: return chunk vector";
  return std::move(_chunkVector);
}

pt::ptree SerializerChunk::GetJson() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: GetJson";
  if (_json.empty()) {
    serialize();
  }

  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: return json";
  return _json;
}

void SerializerChunk::serialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: serialize start";

  _json.put("command", "UploadChunk");
  pt::ptree data;
  for (auto &el : _chunkVector) {
    pt::ptree child;
    child.put("userId", el.userId);
    child.put("chunkId", el.chunkId);
    child.put("sHash", el.sHash);
    child.put("rHash", el.rHash);
    child.put("data", el.data);

    data.push_back(std::make_pair("", child));
  }

  _json.add_child("data", data);
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: serialize stop";
}

void SerializerChunk::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: deserialize start";
  try {
    for (auto &val : _json.get_child("data")) {
      _chunkVector.emplace_back(val.second.get<int>("userId"),
                                val.second.get<int>("chunkId"),
                                val.second.get<std::string>("sHash"),
                                val.second.get<std::string>("rHash"),
                                val.second.get<std::string>("data"));

    }
  } catch (pt::ptree_error &er) {
    BOOST_LOG_TRIVIAL(error) << "SerializerChunk: " << er.what();
    _chunkVector.clear();
    throw SerializerParseException(er.what());
  }

  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: deserialize stop";
}
