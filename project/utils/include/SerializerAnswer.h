#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include <variant>
#include <map>
#include "SerializerInterface.h"
#include "SerializerExceptions.h"

template<class... Ts>
struct overloaded : Ts ... {
  using Ts::operator()...;
};
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct StatusOk {
  explicit StatusOk() = default;
  explicit StatusOk(int id)
      : requestId(id) {
  }

  int requestId{};
};

struct StatusError {
  StatusError() = default;
  StatusError(int id, std::string msg)
      : requestId(id),
        msg(std::move(msg)) {
  }
  StatusError(int id, std::string msg, std::map<int, std::string> errs)
      : requestId(id),
        msg(std::move(msg)),
        errs(std::move(errs)) {
  }

  int requestId{};
  std::string msg;
  std::map<int, std::string> errs;
};

namespace pt = boost::property_tree;

class SerializerAnswer : public SerializerInterface {
 public:
  explicit SerializerAnswer(int id);
  explicit SerializerAnswer(int id, std::string msg);
  explicit SerializerAnswer(int id, std::string msg, std::map<int, std::string> errs);
  explicit SerializerAnswer(const pt::ptree &json);

  std::variant<StatusOk, StatusError> GetStatus() noexcept(false);
  pt::ptree GetJson();

 private:
  void serialize() override;
  void deserialize() override;

 private:
  std::variant<StatusOk, StatusError> _status;
  pt::ptree _json;
};
