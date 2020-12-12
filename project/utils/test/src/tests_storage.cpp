#include "gtest_utils.h"

struct visitor {
  void operator()(const StatusOk &val) const {
    std::cout << "StatusOk: " << val.requestId << std::endl;
  }

  void operator()(const StatusError &val) const {
    std::cout << "StatusError: " << val.requestId << std::endl;
  }
};

TEST(Serializer, AnswerOk) {
  auto sendOK = SerializerAnswer(std::rand() % 100);
  auto sendJsonOK = sendOK.GetJson();
  pt::write_json("ok.json", sendJsonOK);

  pt::ptree recvJsonOK;
  pt::read_json("ok.json", recvJsonOK);
  EXPECT_EQ(sendJsonOK, recvJsonOK);

  auto recvOK = SerializerAnswer(recvJsonOK);
  auto statusOK = recvOK.GetStatus();
  std::visit(visitor{}, statusOK);
}

TEST(Serializer, AnswerError) {
  auto sendERROR = SerializerAnswer(std::rand() % 100, "Error");
  auto sendJsonERROR = sendERROR.GetJson();
  pt::write_json("error.json", sendJsonERROR);

  pt::ptree recvJsonERROR;
  pt::read_json("error.json", recvJsonERROR);
  EXPECT_EQ(sendJsonERROR, recvJsonERROR);

  auto recvERROR = SerializerAnswer(recvJsonERROR);
  auto statusERROR = recvERROR.GetStatus();
  std::visit(visitor{}, statusERROR);
}

TEST(Serializer, AnswerErrorMap) {
  std::map<int, std::string> err;
  for (int i = 0; i < 2; ++i) {
    err.emplace(std::rand() % 100, "op, error");
  }

  auto sendERROR = SerializerAnswer(std::rand() % 100, "Error", err);
  auto sendJsonERROR = sendERROR.GetJson();
  pt::write_json("error.json", sendJsonERROR);

  pt::ptree recvJsonERROR;
  pt::read_json("error.json", recvJsonERROR);
  EXPECT_EQ(sendJsonERROR, recvJsonERROR);

  auto recvERROR = SerializerAnswer(recvJsonERROR);
  auto statusERROR = recvERROR.GetStatus();
  std::visit(visitor{}, statusERROR);
}