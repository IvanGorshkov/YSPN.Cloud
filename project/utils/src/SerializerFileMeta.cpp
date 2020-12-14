#include "SerializerFileMeta.h"
#include <boost/log/trivial.hpp>
#include <utility>

SerializerFileMeta::SerializerFileMeta(int requestId, int userId, FileMeta fileMeta)
    : _requestId(requestId),
      _userId(userId),
      _fileMeta(std::move(fileMeta)) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: create serializer chunk from file";
}

SerializerFileMeta::SerializerFileMeta(const pt::ptree &val)
    : _json(val) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: create serializer chunk from json";
}

int SerializerFileMeta::GetRequestId() const {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: GetRequestId";
  return _requestId;
}

FileMeta SerializerFileMeta::GetFileMeta() noexcept(false) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: GetChunk";
  deserialize();

  return _fileMeta;
}

pt::ptree SerializerFileMeta::GetJson() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: GetJson";
  if (_json.empty()) {
    serialize();
  }

  return _json;
}

void SerializerFileMeta::serialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: serialize";

  _json.put("command", "UploadFile");
  _json.put("requestId", _requestId);
  _json.put("userId", _userId);

  pt::ptree file;
  file.put("fileId", _fileMeta.file.fileId);
  file.put("version", _fileMeta.file.version);
  file.put("fileName", _fileMeta.file.fileName);
  file.put("fileExtension", _fileMeta.file.fileExtension);
  file.put("filePath", _fileMeta.file.filePath);
  file.put("fileSize", _fileMeta.file.fileSize);
  file.put("chunksCount", _fileMeta.file.chunksCount);
  file.put("isCurrent", _fileMeta.file.isCurrent.value());
  file.put("updateDate", _fileMeta.file.updateDate);
  file.put("createDate", _fileMeta.file.createDate);
  _json.add_child("file", file);

  pt::ptree chunks;
  for (auto &el : _fileMeta.chunkMeta) {
    pt::ptree child;
    child.put("chunkId", el.chunkId);

    chunks.push_back(std::make_pair("", child));
  }
  _json.add_child("chunks", chunks);

  pt::ptree fileChunks;
  for (auto &el : _fileMeta.fileChunksMeta) {
    pt::ptree child;
    child.put("chunkId", el.chunkId);
    child.put("chunkOrder", el.chunkOrder);

    fileChunks.push_back(std::make_pair("", child));
  }
  _json.add_child("fileChunks", fileChunks);
}

void SerializerFileMeta::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: deserialize";

  try {
    _requestId = _json.get<int>("requestId");
    _userId = _json.get<int>("userId");

    for (auto &val : _json.get_child("file")) {
      _fileMeta.file = File{.fileId = val.second.get<int>("fileId"),
          .version = val.second.get<int>("version"),
          .fileName = val.second.get<std::string>("fileName"),
          .fileExtension = val.second.get<std::string>("fileExtension"),
          .filePath = val.second.get<std::string>("filePath"),
          .fileSize = val.second.get<int>("fileSize"),
          .chunksCount = val.second.get<int>("chunksCount"),
          .isCurrent = val.second.get<int>("isCurrent"),
          .updateDate = val.second.get<std::string>("updateDate"),
          .createDate = val.second.get<std::string>("createDate")};
    }

    for (auto &val : _json.get_child("chunks")) {
      auto chunkMeta = ChunkMeta{.chunkId = val.second.get<int>("chunkId")};

      _fileMeta.chunkMeta.push_back(chunkMeta);
    }

    for (auto &val : _json.get_child("fileChunks")) {
      auto fileChunkMeta = FileChunksMeta{.chunkId = val.second.get<int>("chunkId"),
          .chunkOrder = val.second.get<int>("chunkOrder")};

      _fileMeta.fileChunksMeta.push_back(fileChunkMeta);
    }
  } catch (pt::ptree_error &er) {
    throw ParseException(er.what());
  }
}
