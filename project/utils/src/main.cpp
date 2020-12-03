#include <iostream>
#include <variant>
#include <boost/property_tree/json_parser.hpp>
#include "SerializerAnswer.h"

struct visitor {
  void operator()(const StatusOk &val) const {
    std::cout << "StatusOk: " << val.requestId << std::endl;
  }

  void operator()(const StatusError &val) const {
    std::cout << "StatusError: " << val.requestId << std::endl;
  }
};

int main() {
  auto sendOK = SerializerAnswer(std::rand() % 100);
  auto jsonOK = sendOK.GetJson();

  pt::write_json("ok.json", jsonOK);


  std::vector<std::pair<int, std::string>> err;
  err.reserve(2);
  for (int i = 0; i < 2; ++i) {
    err.emplace_back(std::rand() % 100, "Ny tyt error");
  }
  auto sendERROR = SerializerAnswer(1, err);
  auto jsonERROR = sendERROR.GetJson();

  pt::write_json("error.json", jsonERROR);


  std::cout << std::endl;


  pt::ptree rootOK;
  pt::read_json("ok.json", rootOK);
  auto recvOK = SerializerAnswer(rootOK);
  auto statusOK = recvOK.GetStatus();
  std::visit(visitor{}, statusOK);

  pt::ptree rootERROR;
  pt::read_json("error.json", rootERROR);
  auto recvERROR = SerializerAnswer(rootERROR);
  auto statusERROR = recvERROR.GetStatus();
  std::visit(visitor{}, statusERROR);

  return 0;
}