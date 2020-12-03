#include "SerializerUserChunk.h"
#include <boost/log/trivial.hpp>

SerializerUserChunk::SerializerUserChunk(std::vector<UserChunk> val)
    : _chunkVector(std::move(val)) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: create serializer chunk from vector";
}

SerializerUserChunk::SerializerUserChunk(const pt::ptree &val)
    : _json(val) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: create serializer chunk from json";
}

std::vector<UserChunk> SerializerUserChunk::GetChunk() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: GetChunk";
  if (_chunkVector.empty()) {
    deserialize();
  }

  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: return vector";
  return std::move(_chunkVector);
}

pt::ptree SerializerUserChunk::GetJson() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: GetJson";
  if (_json.empty()) {
    serialize();
  }

  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: return json";
  return _json;
}

void SerializerUserChunk::serialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: serialize start";

  _json.put("command", "downloadChunk");
  pt::ptree data;
  for (auto &el : _chunkVector) {
    pt::ptree child;
    child.put("userId", el.userId);
    child.put("chunkId", el.chunkId);

    data.push_back(std::make_pair("", child));
  }

  _json.add_child("data", data);
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: serialize stop";
}

void SerializerUserChunk::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: deserialize start";

  for (auto &val : _json.get_child("data")) {
    try {
      _chunkVector.emplace_back(val.second.get<int>("userId"),
                                val.second.get<int>("chunkId"));
    } catch (pt::ptree_error &er) {
      BOOST_LOG_TRIVIAL(error) << "SerializerChunk: " << er.what();
      _chunkVector.clear();
      throw SerializerParseException(er.what());
    }
  }

  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: deserialize stop";
}
