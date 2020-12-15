#include "SerializerFileInfo.h"
#include <boost/log/trivial.hpp>
#include <utility>

SerializerFileInfo::SerializerFileInfo(int requestId, int userId, std::vector<FileInfo> fileMetaVector)
    : _requestId(requestId),
      _userId(userId),
      _fileMetaVector(std::move(fileMetaVector)) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: create serializer chunk from file";
}

SerializerFileInfo::SerializerFileInfo(const pt::ptree &val)
    : _json(val) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: create serializer chunk from json";
}

int SerializerFileInfo::GetRequestId() const {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: GetRequestId";
  return _requestId;
}

std::vector<FileInfo> SerializerFileInfo::GetFileMeta() noexcept(false) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerUserChunk: GetChunk";
  if (_fileMetaVector.empty()) {
    deserialize();
  }

  return _fileMetaVector;
}

pt::ptree SerializerFileInfo::GetJson() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: GetJson";
  if (_json.empty()) {
    serialize();
  }

  return _json;
}

void SerializerFileInfo::serialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: serialize";

  _json.put("command", "UploadFile");
  _json.put("requestId", _requestId);

  pt::ptree data;
  for (auto &fileMeta: _fileMetaVector) {
    pt::ptree child;
    child.put("userId", _userId);

    pt::ptree file;
    file.put("fileId", fileMeta.file.fileId);
    file.put("version", fileMeta.file.version);
    file.put("fileName", fileMeta.file.fileName);
    file.put("fileExtension", fileMeta.file.fileExtension);
    file.put("filePath", fileMeta.file.filePath);
    file.put("fileSize", fileMeta.file.fileSize);
    file.put("chunksCount", fileMeta.file.chunksCount);
    file.put("isCurrent", fileMeta.file.isCurrent.value());
    file.put("updateDate", fileMeta.file.updateDate);
    file.put("createDate", fileMeta.file.createDate);
    child.add_child("file", file);

    pt::ptree chunks;
    for (auto &el : fileMeta.chunkMeta) {
      pt::ptree child;
      child.put("chunkId", el.chunkId);

      chunks.push_back(std::make_pair("", child));
    }
    child.add_child("chunks", chunks);

    pt::ptree fileChunks;
    for (auto &el : fileMeta.fileChunksMeta) {
      pt::ptree child;
      child.put("chunkId", el.chunkId);
      child.put("chunkOrder", el.chunkOrder);

      fileChunks.push_back(std::make_pair("", child));
    }
    child.add_child("fileChunks", fileChunks);

    data.push_back(std::make_pair("", child));
  }

  _json.add_child("files", data);
}

void SerializerFileInfo::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerFileMeta: deserialize";

  try {
    _requestId = _json.get<int>("requestId");
    _userId = _json.get<int>("userId");

    for (auto &fileMeta : _json.get_child("files")) {
      auto oneFileMeta = FileInfo{.userId = fileMeta.second.get<int>("userId")};

      for (auto &val : fileMeta.second.get_child("file")) {
        oneFileMeta.file = FileMeta{.fileId = val.second.get<int>("fileId"),
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

      for (auto &val : fileMeta.second.get_child("chunks")) {
        auto chunkMeta = ChunkMeta{.chunkId = val.second.get<int>("chunkId")};

        oneFileMeta.chunkMeta.push_back(chunkMeta);
      }

      for (auto &val : fileMeta.second.get_child("fileChunks")) {
        auto fileChunkMeta = FileChunksMeta{.chunkId = val.second.get<int>("chunkId"),
            .chunkOrder = val.second.get<int>("chunkOrder")};

        oneFileMeta.fileChunksMeta.push_back(fileChunkMeta);
      }
    }
  } catch (pt::ptree_error &er) {
    throw ParseException(er.what());
  }
}
