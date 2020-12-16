#pragma once

#include <string>

struct FileMeta {
  int fileId;
  int version;
  std::string fileName;
  std::string fileExtension;
  std::string filePath;
  int fileSize;
  int chunksCount;
  bool isDownload;
  bool isDeleted;
  bool isCurrent;
  time_t updateDate;
  time_t createDate;
};