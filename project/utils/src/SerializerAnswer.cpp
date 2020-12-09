#include "SerializerAnswer.h"
#include <boost/log/trivial.hpp>
#include <utility>

SerializerAnswer::SerializerAnswer(int id)
    : _status(StatusOk(id)) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: create serializer answer with OK status";
}

SerializerAnswer::SerializerAnswer(int id, std::string msg)
    : _status(StatusError(id, std::move(msg))) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: create serializer answer with ERROR status";
}

SerializerAnswer::SerializerAnswer(int id, std::string msg, std::map<int, std::string> errs)
    : _status(StatusError(id, std::move(msg), std::move(errs))) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: create serializer answer with ERROR status";
}

SerializerAnswer::SerializerAnswer(const pt::ptree &json)
    : _json(json) {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: create serializer answer from json";
}

std::variant<StatusOk, StatusError> SerializerAnswer::GetStatus() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: GetStatus";
  if (_status.index() == 0) {
    deserialize();
  }

  return _status;
}

pt::ptree SerializerAnswer::GetJson() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: GetJson";
  if (_json.empty()) {
    serialize();
  }

  return _json;
}

void SerializerAnswer::serialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: serialize";

  std::visit(overloaded{
      [&](const StatusOk &val) {
        _json.put("requestId", val.requestId);
        _json.put("status", "OK");
      },
      [&](const StatusError &val) {
        _json.put("requestId", val.requestId);
        _json.put("status", "ERROR");
        _json.put("msg", val.msg);

        pt::ptree errors;

        for (auto &er : val.errs) {
          pt::ptree child;
          child.put("id", er.first);
          child.put("msg", er.second);

          errors.push_back(std::make_pair("", child));
        }

        _json.add_child("errors", errors);
      }
  }, _status);
}

void SerializerAnswer::deserialize() {
  BOOST_LOG_TRIVIAL(debug) << "SerializerAnswer: deserialize";

  try {
    auto status = _json.get<std::string>("status");
    if (status == "OK") {
      auto id = _json.get<int>("requestId");
      _status = StatusOk(id);

    } else {
      auto id = _json.get<int>("requestId");
      auto msg = _json.get<std::string>("msg");
      std::map<int, std::string> errs;

      for (auto &val : _json.get_child("errors")) {
        auto erId = val.second.get<int>("id");
        auto erMsg = val.second.get<std::string>("msg");
        errs.emplace(erId, erMsg);
      }

      _status = StatusError(id, msg, errs);
    }
  } catch (pt::ptree_error &er) {
    throw ParseException(er.what());
  }
}
