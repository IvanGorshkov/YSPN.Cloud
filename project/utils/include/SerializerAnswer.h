#pragma once

#include "SerializerInterface.h"
#include "SerializerExceptions.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include <variant>

template<class... Ts>
struct overloaded : Ts ... {
  using Ts::operator()...;
};
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct StatusOk {
  StatusOk() = default;
  explicit StatusOk(int id)
      : requestId(id) {
  }

  int requestId{};
};

struct StatusError {
  StatusError() = default;
  StatusError(int id, std::vector<std::pair<int, std::string>> errs)
      : requestId(id),
        errs(std::move(errs)) {
  }

  int requestId{};
  std::vector<std::pair<int, std::string>> errs;
};

namespace pt = boost::property_tree;

class SerializerAnswer : public SerializerInterface {
 public:
  explicit SerializerAnswer(int id);
  explicit SerializerAnswer(int id, std::vector<std::pair<int, std::string>> errs);
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
