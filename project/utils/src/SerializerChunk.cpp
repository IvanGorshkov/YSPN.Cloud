#include "SerializerChunk.h"
#include <boost/log/trivial.hpp>

SerializerChunk::SerializerChunk(int requestId, std::vector<Chunk> val)
    : _requestId(requestId),
      _chunkVector(std::move(val)) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: create serializer chunk from vector";
}

SerializerChunk::SerializerChunk(const pt::ptree &val)
    : _json(val) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: create serializer chunk from json";
}

int SerializerChunk::GetRequestId() const {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: GetRequestId";
  return _requestId;
}

std::vector<Chunk> SerializerChunk::GetChunk() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: GetChunk";
  if (_chunkVector.empty()) {
    deserialize();
  }

  return std::move(_chunkVector);
}

pt::ptree SerializerChunk::GetJson() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: GetJson";
  if (_json.empty()) {
    serialize();
  }

  return _json;
}

void SerializerChunk::serialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: serialize";

  _json.put("command", "UploadChunk");
  _json.put("requestId", _requestId);
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
}

void SerializerChunk::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: deserialize";

  try {
    _requestId = _json.get<int>("requestId");
    for (auto &val : _json.get_child("data")) {
      _chunkVector.emplace_back(val.second.get<int>("userId"),
                                val.second.get<int>("chunkId"),
                                val.second.get<std::string>("sHash"),
                                val.second.get<std::string>("rHash"),
                                val.second.get<std::string>("data"));

    }
  } catch (pt::ptree_error &er) {
    _chunkVector.clear();
    throw ParseException(er.what());
  }
}
