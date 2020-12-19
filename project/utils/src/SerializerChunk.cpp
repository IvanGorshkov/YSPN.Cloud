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
    child.put("chunkSize", el.chunkSize);
    child.put("sHash", el.sHash);
    child.put("rHash", el.rHash);
    std::string s(el.data.data(), el.data.size());
    child.put("data", s);

    data.push_back(std::make_pair("", child));
  }

  _json.add_child("data", data);
}

void SerializerChunk::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerChunk: deserialize";

  try {
    _requestId = _json.get<int>("requestId");
    for (auto &val : _json.get_child("data")) {
      auto chunk = Chunk{.userId = val.second.get<int>("userId"),
          .chunkId =  val.second.get<int>("chunkId"),
          .chunkSize =  val.second.get<int>("chunkSize"),
          .sHash = val.second.get<std::string>("sHash"),
          .rHash =  val.second.get<std::string>("rHash")
      };

      auto str = val.second.get<std::string>("data");
      std::vector<char> vec;
      std::for_each(str.begin(), str.end(), [&vec](char &el) {
        vec.push_back(el);
      });
      chunk.data = vec;

      _chunkVector.push_back(chunk);
    }
  } catch (pt::ptree_error &er) {
    throw ParseException(er.what());
  }
}
