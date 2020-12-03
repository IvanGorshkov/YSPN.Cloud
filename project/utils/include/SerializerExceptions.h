#pragma once

#include <exception>
#include <string>
#include <utility>

class SerializerParseException : public std::exception {
 public:
  explicit SerializerParseException(std::string msg)
      : _msg(std::move(msg)) {
  }

  const char *what() const noexcept override {
    return _msg.c_str();
  }

 private:
  std::string _msg;
};