#include "ClientNetwork.h"
#include <iostream>

namespace pt = boost::property_tree;

int main(int argc, char *argv[]) {
  ClientNetwork client;
  client.Connect();

  srand(time(0));

  pt::ptree root;
  int id = 2;

  root.put("command", "UploadChunk");
  root.put("requestId", id);
  pt::ptree data;
  for (int i = 0; i < 2; i++) {
    pt::ptree child;
    child.put("userId", id);
    child.put("chunkId", i);
    child.put("sHash", "shash");
    child.put("rHash", "rhash");
    child.put("data", "data");

    data.push_back(std::make_pair("", child));
  }

  root.add_child("data", data);

  client.SendJSON(root);

  pt::ptree response = client.ReceiveJSON();

  std::stringstream ss;
  boost::property_tree::write_json(ss, response);
  std::cout << "Reply is: " << ss.str() << std::endl;

  exit(EXIT_SUCCESS);
}