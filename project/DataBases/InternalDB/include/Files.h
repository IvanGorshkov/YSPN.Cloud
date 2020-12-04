#pragma once

struct Files {
  std::string file_name;
  std::string file_extention;
  size_t file_size;
  std::string file_path;
  size_t count_chunks;
  size_t version;
  bool is_download;
  std::string update_date;
  std::string create_date;
};