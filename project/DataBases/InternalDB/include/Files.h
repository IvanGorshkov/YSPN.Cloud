#pragma once
#include <optional>
struct Files {
  size_t id;
  std::string file_name;
  std::string file_extention;
  size_t file_size;
  std::string file_path;
  size_t count_chunks;
  size_t version;
  std::optional<bool> is_download;
  std::optional<bool> is_current;
  std::string update_date;
  std::string create_date;
};