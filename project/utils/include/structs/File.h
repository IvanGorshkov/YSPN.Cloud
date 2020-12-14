#pragma once

#include <optional>

struct File {
  int fileId;
  int version;
  std::string fileName;
  std::string fileExtension;
  std::string filePath;
  int fileSize;
  int chunksCount;
  std::optional<bool> isDownload;
  std::optional<bool> isCurrent;
  std::string updateDate;
  std::string createDate;
};