#pragma once

#include <exception>
#include <string>
#include <utility>

class AppExceptions : public std::exception {
 public:
  explicit AppExceptions(std::string msg)
      : _msg(std::move(msg)) {
  }

  const char *what() const noexcept override {
    return _msg.c_str();
  }

 private:
  std::string _msg;
};

class WrongFileNum : public AppExceptions {
 public:
  explicit WrongFileNum(std::string msg)
      : AppExceptions("Wrong file num: " + std::move(msg)) {
  }
};