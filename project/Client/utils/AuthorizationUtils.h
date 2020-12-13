#pragma once

#include <string>

struct UserIdPassword {
  UserIdPassword(int userId, const std::string &password)
      : userId(userId),
        password(password) {
  }

  int userId;
  std::string password;
};

struct UserIdOldNewPassword {
  UserIdOldNewPassword(int userId, const std::string &oldPassword, const std::string &newPassword)
      : userId(userId),
        oldPassword(oldPassword),
        newPassword(newPassword) {
  }

  int userId;
  std::string oldPassword;
  std::string newPassword;
};

struct User {
  User(const std::string &login, const std::string &password, const std::string &deviceName)
      : login(login),
        password(password),
        deviceName(deviceName) {
  }

  std::string login;
  std::string password;
  std::string deviceName;
};

struct UserIdDeviceId {
  UserIdDeviceId(int userId, int deviceId)
      : userId(userId),
        deviceId(deviceId) {
  }

  int userId;
  int deviceId;
};

