#pragma once

#include <string>

struct User {
  size_t userId;
  std::string login;
  std::string password;
  size_t deviceId;
  std::string deviceName;
  std::string syncFolder;
  std::string lastUpdate;
};
